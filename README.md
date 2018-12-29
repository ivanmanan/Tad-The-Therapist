# Tad-The-Therapist
Code for senior design project. Design project was broken down to
two segments\: the LCDK and the web server.


## LCDK
The LCDK does all the speech signal processing. It then determines an appropriate response, which is transmitted as a `.txt` file onto the developer machine.


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

## Audio Response
Use the Free Text to MP3 website to download MP3 files. Also copy and paste responses as a `txt` file and have the same file name as the `mp3` file.
* https://www.texttomp3.online/
* https://www.text2speech.org/ (Provides wav file; choose slow speed)

