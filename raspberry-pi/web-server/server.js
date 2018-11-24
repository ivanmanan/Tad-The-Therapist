// Server

/////////////////////////////////////////////////////////////////////
// Modules
const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const debug = require('./debug');

/////////////////////////////////////////////////////////////////////
// Configurations

// Parsers for HTTP Methods
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

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
// Start Application

// Set port
var port = process.env.PORT || 3001;

// NOTE: For release, run 'npm run build' in client directory
//       The site is running on port 3001
const path = require('path');
app.use(express.static(path.join(__dirname, 'client/build')));

app.listen(port);

console.log('Server running on port 3001...');
