// Server

/////////////////////////////////////////////////////////////////////
// Modules
const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const say = require('say');
const debug = require('./debug');

/////////////////////////////////////////////////////////////////////
// Configurations

// Parsers for HTTP Methods
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


// File path holding all the txt files may change
// TODO: May change this to be relative to where server.js is
const FILE_PATH = "/mnt/c/therapist";

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


/////////////////////////////////////////////////////////////////////
// HTTP Methods

// Retrieve entirety of table to obtain list of Time Stamps
// Also create hash table that maps time stamp to file name
app.get('/conversations', (req, res) => {
  console.log("Running query...");
  const query = 'SELECT * FROM Conversations;';
  console.log(query);

  // Test text2speech here
  // This may had caused all the errors previously...
  //say.speak("Hello!");

  // Must test this query by inserting fake data and text files into the MySQL database
  connection.query(query, (err, savedConversations, fields) => {
    try {
      if (err) throw err;

      // ERROR: This sends an object, but we want to send an array of strings from one field
      res.contentType('application/json');
      res.send(JSON.stringify(savedConversations));
    }
    catch(err) {
      console.log("ERROR EXISTS.");
      const empty = [];
      res.contentType('application/json');
      res.send(JSON.stringify(empty));
    }
  });

});


// Insert into the MySQL database as well as append to the hash table
// Insert query can retrieve ID number of MySQL table
app.post('/conversation', (req, res) => {
  console.log("Running query...");
  const timeStamp = req.body.timeStamp;
  const dialogue = req.body.currentDialogue;

  // TODO: Work on creating the text file first; figure out naming convention for File_Name, then create the INSERT query

  // First create and/or append text file, then add file name onto query
  const query_one = 'INSERT INTO Conversations (Time_Stamp, File_Name) VALUES ("';
  console.log(query_one);

  

  // Do text2speech here
  say.speak("Hello World!");
});



// Access the hash table global variable to find selected time stamp and its associated text file
app.get('/conversation', (req, res) => {
  console.log("Do nothing for now");
});


// Delete conversation from table and from the hash table
app.delete('/conversation', (req, res) => {
  console.log("Do nothing for now");
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
