#ifndef CHATBOT_H
#define CHATBOT_H


// NOTE: May need to create a data structure to keep track of conversation history
//       OR I can read from output.txt

#include <iostream>
#include <fstream>
//#include something for connecting to web server

using namespace std;

class Chatbot {
public:
	void respond(const string& phrase) const;    // Used to get, or teach a response
	void say(string phrase);    // Used to textually and audibly communicate a phrase
};

#endif