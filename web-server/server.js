// Server

/////////////////////////////////////////////////////////////////////
// Modules
const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const say = require('say');
const fs = require('fs');
const debug = require('./debug');

/////////////////////////////////////////////////////////////////////
// Configurations

// Parsers for HTTP Methods
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


// NOTE: File path holding all the txt files may change
const FILE_PATH = "/mnt/c/therapist/lcdk/output/";

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
  console.log('You are now connected to the MySQL Database.')
})

// Global variable that stores all of the MySQL content into a cache
let Conversations_Cache = {};

/////////////////////////////////////////////////////////////////////
// Event Listener

// TODO: Create socket method that detects changes in a .txt file
//       Changes in .txt file gets added to the socket
// TODO: Implement the say.js package to voice the dialogue
//       See the test-speak.js file on how this is done

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
app.post('/conversation', (req, res) => {
  console.log("Running query...");
  const timeStamp = req.body.timeStamp;
  const dialogue = req.body.currentDialogue;

  // TODO: Work on creating the text file first; figure out naming convention for File_Name, then create the INSERT query
  // Need to obtain last ID number that exists in the database so I can append as the convo#.txt file name

  // First create and/or append text file, then add file name onto query
  const query_one = 'INSERT INTO Conversations (Time_Stamp, File_Name) VALUES ("';
  console.log(query_one + '\n');

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



/////////////////////////////////////////////////////////////////////
// Start Application

// Set port
var port = process.env.PORT || 3001;

// NOTE: For release, run 'npm run build' in client directory
//       The site is running on port 3001
const deploy = false;
if (deploy) {

  // TODO: Add code that automates the 'npm run build' in client directory
  //       Afterwards, insert this change into React-Setup repository

  const path = require('path');
  app.use(express.static(path.join(__dirname, 'client/build')));
}

app.listen(port);

console.log('Server running on port 3001...');
