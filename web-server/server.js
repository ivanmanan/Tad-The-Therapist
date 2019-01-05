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

/////////////////////////////////////////////////////////////////////
// Configurations

// Parsers for HTTP Methods
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


// NOTE: Windows PowerShell use Windows file path
// NOTE: WSL use regular file path
//const FILE_PATH = "/mnt/c/therapist/lcdk/output/";
const FILE_PATH = "C:\\therapist\\lcdk\\output\\";
const OUTPUT_FILE = "output.txt";

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

// NOTE: For release, run 'npm run build' in client directory
//       The site is running on port 3001
const deploy = false;
if (deploy) {
  // TODO: Add code that automates the 'npm run build' in client directory
  //       Afterwards, insert this change into React-Setup repository
  const path = require('path');
  app.use(express.static(path.join(__dirname, 'client/build')));
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
watch(FILE_PATH + OUTPUT_FILE, (event, filename) => {
  if (event === 'update') {
    // Read the file and retrieve last two lines
    const CONVO = FILE_PATH + OUTPUT_FILE;
    const contents = fs.readFileSync(CONVO).toString().split("\n");
    const SPEECH_SPEED = 1;
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


// TODO: Insert into the MySQL database as well as append to the hash table --> this is just Conversations_Cache[key] = value
// Insert query can retrieve ID number of MySQL table
// TODO: POST request ends conversation and no new content should be added into dialogue
app.post('/conversation', (req, res) => {
  console.log("Running query...");
  const time_stamp = req.body.time_stamp;
  const query_one = 'SELECT LAST_INSERT_ID();';
  console.log(query_one);
  connection.query(query_one, (err, result, fields) => {
    // TODO: Work on creating the text file first;  
    // Need to console.log what result outputs
    debug(result);
    // Need to increment last ID by 1 so I can have a new convo text file
    // Copy the active text file and rename as convo#.txt
    // Then insert into the MySQL database
    // Update the hash table

    // First create and/or append text file, then add file name onto query
    const query_two = 'INSERT INTO Conversations (Time_Stamp, File_Name) VALUES ("';
    console.log(query_two + '\n');
    // TODO: POST request sends the same content from the GET request -- maintain data integrity
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