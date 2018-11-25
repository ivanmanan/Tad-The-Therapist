# Tad-The-Therapist
Code for senior design project. Design project was broken down to
two segments\: the LCDK and the Raspberry Pi.

## LCDK
Create API that communicates data onto the Raspberry Pi.
Back-up plan: export the file as a .txt onto the developer machine
and have the server detect what data was transmitted.

## Raspberry Pi
The Raspberry Pi is divided into two main directories: the speech output response and a web server. A third directory may be added
to assist in the API communication between the LCDk and the
Raspberry Pi.

### Output Audio Response
Use data transmitted from the LCDK to output an appropriate
response. The data is used to pick the correct audio file to
play. Likewise, the corresponding `txt` file will be displayed on
the web browser.

Output can be played using a python script that plays the mp3 or
wav file. Use omxplayer.

### Web
Web server is to display conversation history and view previous
conversations on a web browser.

Raspberry Pi should start web server automatically at boot. This
needs to be tested on the UCLA Web wi-fi.

Use MySQL database to [store conversation
history](https://stackoverflow.com/questions/6472233/can-i-store-images-in-mysql). Keep
conversations as `txt` files and store path names to the
database. Also date and time of the conversations.

I can also use the MySQL database to store path names of the audio
files.
