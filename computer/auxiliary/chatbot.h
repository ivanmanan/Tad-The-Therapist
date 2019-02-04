#ifndef CHATBOT_H
#define CHATBOT_H

// NOTE: May need to create a data structure to keep track of conversation history
//       OR I can read from output.txt

#include <iostream>
#include <fstream>

using namespace std;

/*
	TODO: I can create a hash table and have some probabilistic distribution
		  I want to identify keywords within a phrase
		  Have text file with key words -- if found, then have the hash table display it

	The following function will look for keywords in the vocabulary file.
	If there is a match, the accompanying response, stored below the initial phrase,
	will be outputed.
	If the response cannot be found, the learner will repeat the phrase, and prompt
	the user to enter an ideal response. This response will be stored in the vocabulary
	file along with the initial phrase.
*/

void respond(const string& phrase) {
	fstream vocabulary;
	vocabulary.open("vocabulary.txt", ios::in);    // Open the vocabulary file for input

	// Search through the file until the end is reached
	while (!vocabulary.eof()) {    // While not at end of file
		string identifier;
		getline(vocabulary, identifier);    // Get next phrase

		if (identifier == phrase) {    // Is it the phrase we are looking for
			string response;
			getline(vocabulary, response);   // If so, get the response
			cout << response << endl;   // Textually output the response
			return;    // Leave the function
		}
		getline(vocabulary, identifier);	//skip response line in vocabulary.txt
	}

	vocabulary.close();    // Looks like we couldn't find the phrase in vocabulary. Close the file!
	vocabulary.open("vocabulary.txt", ios::out | ios::app);    // Now open for output, and append at end of file
	vocabulary << phrase << endl;    // Record initial phrase in vocabulary

	cout << "Phrase not found, input response" << endl;   // Repeat the phrase the user entered
	string userResponse;
	cout << "RESPONSE: ";
	getline(cin, userResponse);    // Get the ideal response
	vocabulary << userResponse << endl;    // Write the ideal response to vocabulary
	vocabulary.close();    // Close the file!
}

#endif