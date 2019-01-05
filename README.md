# To-Do List
* Display the Web user interface onto a big screen so I can assess the Bootstrap scaling and whether everything will look fine in a bigger screen other than my laptop

# Tad-The-Therapist
Code for senior design project. The design project was broken down into
two segments\: the LCDK and the web server.


## LCDK
The LCDK does all the speech signal processing. The LCDK then outputs its speech recognition into a `.txt` file onto the developer machine. The LCDK also determines the appropriate response and appends it into the `.txt` file. The `.txt` file is located in the `/lcdk/output/` folder. The LCDK practically contains its own dictionary.


## Web Server
The developer machine hosts a web server that reads the content of the `.txt` file. The server contains all the speech output responses that are to be played. Moreover, the frontend displays the entire conversation history.


### Output Audio Response
Use data transmitted from the LCDK to output an appropriate speech response. The data is used to pick the correct audio file to play. Likewise, the corresponding `.txt` file that subtitles the audio file will be displayed on the web browser.

Output can be played using a python script that plays the mp3 or wav file. Use omxplayer.

### Web
Web server is to display conversation history and view previous conversations on a web browser.

Use MySQL database to [store conversation
history](https://stackoverflow.com/questions/6472233/can-i-store-images-in-mysql). Keep
conversations as `txt` files and store path names to the
database. Also date and time of the conversations.

I can also use the MySQL database to store path names of the audio
files.


### Frontend: User Interface
├── App.jsx<br/>
├── index.js<br/>
├── **Components**<br/>
│   ├── Clock.jsx<br/>
│   ├── Sidebar.jsx<br/>
│   └── Conversation.jsx<br/>
│   └── Oldconversation.jsx<br/>
└── styles.css<br/>


## Audio Response
Playing audio in Windows machine:
1. If you are using **wsl**, then you must follow [this guide](https://token2shell.com/howto/x410/enabling-sound-in-wsl-ubuntu-let-it-sing/). This guide does not work properly. So just switch to use PowerShell for audio development
2. If you are using Windows **PowerShell**, then you must follow [this guide](https://www.youtube.com/watch?v=My7im5WIwrQ). For audio development purposes, you must use PowerShell.


## Setting Up MySQL on VS Code
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


## Infrequent Windows Glitch
If the Node.js server is running properly but the frontend browser is not loading, then you may have to kill all other existing Node.js processes.
```bash
ps aux | grep node
kill -9 PID PID PID
```


## Scenarios to Consider
* What if user clicks on past conversation in the middle of an active convseration?
    * Then the user abandoned the current conversation.
* What if the user refreshes in the middle of a conversation?
    * Then the user abandoned the conversation.
* How do we know when the converation is over?
    * The `.txt` file stops obtaining appended messages. User decides to save conversation or to not save the conversation.

