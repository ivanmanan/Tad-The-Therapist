// Server

/////////////////////////////////////////////////////////////////////
// Modules
const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const say = require('say');
const fs = require('fs');
const watch = require('node-watch');
const debug = require('./debug');
const path = require('path');

/////////////////////////////////////////////////////////////////////
// Configurations

// Parsers for HTTP Methods
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


// NOTE: WSL use regular file path
// NOTE: Windows PowerShell use Windows file path
//const FILE_PATH = "/mnt/c/therapist/computer/output/";
const FILE_PATH = "C:\\therapist\\computer\\output\\";
const OUTPUT_FILE = FILE_PATH + "output.txt";

/////////////////////////////////////////////////////////////////////
// MySQL Database

var mysql = require('mysql');
config = require("./config");
db = config.database;

var connection = mysql.createConnection({
  host: db.host,
  user: db.user,
  password: db.password,
  database: db.database
})

connection.connect(function(err) {
  if (err) throw err
  console.log('You are now connected to the MySQL Database.\n')
})

// Global variable that stores all of the MySQL content into a cache
let Conversations_Cache = {};

/////////////////////////////////////////////////////////////////////
// Start Application

// NOTE: For release, the site is running on port 3001
const deploy = true;
if (deploy) {
  // This automates the 'npm run build' in client directory
  const exec = require('child_process').exec;
  const child = exec('npm run build --prefix client', (err) => {
    app.use(express.static(path.join(__dirname, 'client/build')));
  });
}

// Set port
var port = process.env.PORT || 3001;
const server = app.listen(port, () => {
  console.log("Server running on port 3001...\n");
});

/////////////////////////////////////////////////////////////////////
// Event Listener
const io = require('socket.io')(server);

// This function runs every time the client refreshes
io.sockets.on('connection', socket => {
  console.log("Socket.io listening on port 3001...\n");
  socket.emit('INITIALIZE', {message: 'Socket channel active between server and client...'});
});

// Socket emits new dialogue every time file changes
watch(OUTPUT_FILE, (event, filename) => {
  if (event === 'update') {
    // Read the file and retrieve last two lines
    const CONVO = OUTPUT_FILE;
    const contents = fs.readFileSync(CONVO).toString().split("\n");
    const SPEECH_SPEED = 1.25;
    if (contents.length === 1) {
      // This is initializing the conversation
      const tad = contents[0];
      say.speak(tad, 'Microsoft Zira Desktop', SPEECH_SPEED);
      console.log("Tad: " + tad);
      io.emit('DIALOGUE', {tad: tad, client: ""});
    }
    else if (contents.length < 1) {
      // NOTE: This case should never happen based on property of the embedded device
      return;
    }
    else {
      const tad = contents[contents.length-1];
      const client = contents[contents.length-2];
      say.speak(tad, 'Microsoft Zira Desktop', SPEECH_SPEED);
      console.log("Client: " + client);
      setTimeout(() => console.log("Tad: " + tad), 700);
      io.emit('DIALOGUE', {tad: tad, client: client});
    }
  }
});

/////////////////////////////////////////////////////////////////////
// HTTP Methods

// Retrieve entirety of table to obtain list of Time Stamps
// Also creates hash table that maps time stamp to file name
app.get('/conversations', (req, res) => {
  console.log("Running query...");
  const query = 'SELECT * FROM Conversations;';
  console.log(query + '\n');
  connection.query(query, (err, savedConversations, fields) => {
    try {
      if (err) throw err;
      // Place the MySQL table into a cache
      for (let i = 0; i < savedConversations.length; i++) {
        let temp_timeStamp = savedConversations[i].Time_Stamp;
        let temp_fileName = savedConversations[i].File_Name;
        Conversations_Cache[temp_timeStamp] = temp_fileName;
      }
      res.contentType('application/json');
      res.send(JSON.stringify(savedConversations));
    }
    catch(err) {
      console.log("ERROR: " + err + "\n");
      const empty = [];
      res.contentType('application/json');
      res.send(JSON.stringify(empty));
    }
  });
});


// Insert into the MySQL database as well as append to the hash table
app.post('/conversation', (req, res) => {
  console.log("\nRunning query...");
  const time_stamp = req.body.time_stamp;
  const query_one = 'INSERT INTO Conversations (Time_Stamp, File_name) VALUES ("' + time_stamp + '", "TEMP_NULL");';
  
  console.log(query_one);
  connection.query(query_one, (err_one, result_one, fields) => {
    try {
      if (err_one) throw err_one;
      const append_number = result_one.insertId;
      const new_file_name = "convo" + append_number + ".txt";
      const query_two = 'UPDATE Conversations SET File_Name = "' + new_file_name + '" WHERE Time_Stamp="' + time_stamp + '";';
      console.log(query_two + '\n');
      connection.query(query_two, (err_two, result_two, fields) => {
        try {
          if (err_two) throw err_two;
          // Copy output.txt and save it as convo#.txt
          fs.copyFile(OUTPUT_FILE, FILE_PATH + new_file_name, (err) => {
            if (err) throw err;
            console.log("New file was created: " + new_file_name);
          });
          
          // Update the hash table
          Conversations_Cache[time_stamp] = new_file_name;
          res.end();          
        }
        catch(err_two) {
          console.log("ERROR: " + err_two);
          res.end();    
        }
      })
    }
    catch(err_one) {
      // If error exists, then run query that removes potentially duplicate values in table
      console.log("ERROR: " + err_one);
      const query_three = 'DELETE FROM Conversations WHERE File_Name="TEMP_NULL";';
      connection.query(query_three, () => {
        res.end();
      });
    }
  });
});


// Access the cache to find selected time stamp and its associated text file
app.get('/conversation/:timeStamp', (req, res) => {
  const time_stamp = req.params.timeStamp;
  console.log("Retrieving from hash table...");
  const file_name = Conversations_Cache[time_stamp];
  // If the file name exists in the cache
  if (typeof(file_name) !== undefined) {
    console.log("Key: " + time_stamp + "; Value: " + file_name + "\n");
    // Retrieve the file and send it to frontend
    const CONVO = FILE_PATH + file_name;
    const contents = fs.readFileSync(CONVO).toString().split("\n");
    res.contentType('application/json');
    res.send(contents);
  }
  else {
    console.log("ERROR: The requested time stamp does not exist in the cache.\n")
    res.end();
  }
});


// Delete conversation from table and from the hash table
app.delete('/conversation', (req, res) => {
  const time_stamp = req.body.time_stamp;
  console.log("Running query...");
  const query = 'DELETE FROM Conversations WHERE Time_Stamp="' + time_stamp + '";';
  console.log(query + '\n');
  connection.query(query, (err, result, fields) => {
    try {
      if (err) throw err;
      // Retrieve from cache file name
      // Remove the existing text file from the folder
      console.log("Retrieving from hash table...");
      const file_name = Conversations_Cache[time_stamp];
      // If the file name exists in the cache
      if (typeof(file_name) !== undefined) {
        console.log("Key: " + time_stamp + "; Value: " + file_name + "\n");
        // Retrieve the file and remove it from server
        const CONVO = FILE_PATH + file_name;
        fs.unlink(CONVO, (err) => {
          if (err) throw err;
          console.log("The following file has been deleted: " + CONVO + "\n");
        })
      }
      else {
        console.log("ERROR: The requested time stamp does not exist in the cache.\n")
        res.end();
      }
      // Update cache
      delete Conversations_Cache[time_stamp];
      // Let frontend remove time stamp from state
      res.end()
    }
    catch(err) {
      console.log("ERROR: " + err + '\n');
      res.end();
    }
  });
});