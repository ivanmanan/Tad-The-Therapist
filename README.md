# To-Do List
* Display the Web user interface onto a big screen so I can assess the Bootstrap scaling and whether everything will look fine in a bigger screen other than my laptop
* Consider implementing recurrent neural network in Python if we decided not to use HMM's in C++
* Research on dynamic time warping
* Consider using spectrograms instead of MFCC's
* Figure out how to implement a chatbot using a machine learning method for Tad the Therapist
* Can develop neural network on my own computer using the mp3 files
* Switch --- pass first response into the text file
* C++ program detect file changes constantly
* Read how to implement an ML method chatbot

# Tad-The-Therapist
Code for senior design project. The design project was broken down into
three segments\: the LCDK, the computer program, and the web server.


## LCDK
The LCDK does all the speech signal processing. The LCDK then outputs its MFCC spectrum into a `.txt` file onto the developer machine. The `.txt` file is located in the `/lcdk/output/` folder.


## Computer
The computer program has both `train` and `main` functionalities. 


The `train` program trains a given data set and exports it as `training_set.txt`.


The `main` program does a trial speech recognition given a `.txt` file from the LCDK and the `training_set.txt` file from the `train` program. The `main` program does speech recognition via hidden markov models and export the identified speech as a `.txt` file. The computer program also determines an appropriate response and appends it onto the same `.txt` file. The `.txt` file is located in the `/computer/output/` folder.


## Web Server
The developer machine hosts a web server that reads the content of the `.txt` file made by the computer program. The server plays the speech output via the `npm` `say.js` package. Moreover, the frontend displays conversation history.


### Frontend: User Interface
├── App.jsx<br/>
├── index.js<br/>
├── **Components**<br/>
│   ├── Clock.jsx<br/>
│   ├── Sidebar.jsx<br/>
│   └── Conversation.jsx<br/>
│   └── Oldconversation.jsx<br/>
└── styles.css<br/>


### Audio Response
You must use Windows **PowerShell** for the `Node.js` server to playback audio on a Windows machine.


### Setting Up MySQL on VS Code
Install MySQL via `sudo apt install mysql-server`.

When you initially attempt to run `mysql -u root -p`, you obtain the following error message:
```bash
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (2)
```

Run the following commands to reset your MySQL password. You may not be able to use special characters in your password, such as exclamation marks.
```bash
sudo /etc/init.d/mysql stop
sudo service mysql start 
sudo mysqld_safe --skip-grant-tables &
mysql -u root

mysql> use mysql;
mysql> update mysql.user set authentication_string=PASSWORD('my_password') where user='root' and host='localhost';
mysql> quit;
sudo mysqladmin shutdown
sudo service mysql start
```

Access the MySQL database via this command.
```bash
mysql -u root -p
```

At startup, you may need to run the following command to initialize the MySQL server.
```bash
sudo service mysql start
```


### Infrequent Windows Glitch
If the `Node.js` server is running properly but the frontend browser is not loading, then you may have to kill all other existing `Node.js` processes.
```bash
ps aux | grep node
kill -9 PID PID PID
```


### Scenarios to Consider
* What if user clicks on past conversation in the middle of an active convseration?
    * Then the user abandoned the current conversation.
* What if the user refreshes in the middle of a conversation?
    * Then the user abandoned the conversation.
* How do we know when the converation is over?
    * The `.txt` file stops obtaining appended messages. User decides to save conversation or to not save the conversation.


## File Names
├── **lcdk**<br/>
│   ├── start.txt<br/>
│   ├── input.txt<br/>
│   └── done.txt<br/>
└── **computer**<br/>
    ├── busy.txt<br/>
    └── **output**<br/>
        ├── output.txt<br/>
        └── convo#.txt<br/>


**/lcdk/start.txt** - Initialize therapy session for C++ program
**/lcdk/input.txt** - Feature that goes into the neural network
**/lcdk/done.txt** - Computer program detects if new value in file is different from current value, then reads the new input text file
**/computer/busy.txt** - LCDK reads this file to trigger button usability based on whehter computer is idle or not
**/computer/output/output.txt** - Used to display current chat history in web interface.
**/computer/output/convo#.txt** - Used to store conversation history.