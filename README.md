# TO-DO List
* LCDK --> `.txt` file --> Web Server
    * What is the content of this `.txt` file?


# Tad-The-Therapist
Code for senior design project. Design project was broken down to
two segments\: the LCDK and the web server.


## LCDK
The LCDK does all the speech signal processing. It then determines an appropriate response, which is transmitted as a `.txt` file onto the developer machine. The `.txt` file is located in the `/lcdk/output/` folder.


## Web Server
The developer machine hosts a web server that reads the content of the `.txt` file. The server also contains all the speech output responses that are to be played. Moreover, the frontend displays the entire conversation history.


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
└── styles.css<br/>


## Audio Response
Use the Free Text to MP3 website to download MP3 files. Also copy and paste responses as a `txt` file and have the same file name as the `mp3` file.
* https://www.texttomp3.online/
* https://www.text2speech.org/ (Provides wav file; choose slow speed)


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