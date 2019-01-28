#include "Chatbot.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ios>

using namespace std;
/*
	The following function will look for the passed phrase in the memory file.
	If there is a match, the accompanying response, stored below the initial phrase,
	will be outputed.
	If the response cannot be found, the learner will repeat the phrase, and prompt
	the user to enter an ideal response. This response will be stored in the memory
	file along with the initial phrase.
*/
void Chatbot::respond(const string& phrase) const {
	fstream memory;
	memory.open("memory.txt", ios::in);    // Open the memory file for input

	// Search through the file until the end is reached
	while (!memory.eof()) {    // While not at end of file
		string identifier;
		getline(memory, identifier);    // Get next phrase

		if (identifier == phrase) {    // Is it the phrase we are looking for
			string response;
			getline(memory, response);   // If so, get the response
			cout << response << endl;   // Textually output the response
			return;    // Leave the function
		}
		getline(memory, identifier);	//skip response line in memory.txt
	}

	memory.close();    // Looks like we couldn't find the phrase in memory. Close the file!
	memory.open("memory.txt", ios::out | ios::app);    // Now open for output, and append at end of file
	memory << phrase << endl;    // Record initial phrase in memory

	cout << "Phrase not found, input response" << endl;   // Repeat the phrase the user entered
	string userResponse;
	cout << "RESPONSE: ";
	getline(cin, userResponse);    // Get the ideal response
	memory << userResponse << endl;    // Write the ideal response to memory
	memory.close();    // Close the file!
}

/*
	This function simply communicates a phrase textually and audibly
*/
void Chatbot::say(string phrase) {
	cout << phrase << endl;
}