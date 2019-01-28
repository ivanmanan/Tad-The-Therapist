#include <iostream>
#include <string>
#include "Chatbot.h"

using namespace std;

int main() {
	Chatbot Tad;    // Create a learner object

	cout << "Tad: Hi, my name is Tad, how can I help you today?";
	/*
		The following is the main loop. It will continue until the application is closed.
		The user enters their input, and then the learner will respond.
	*/
	for (;;) {
		cout << "\nUser: ";    // User prompt
		string phrase;
		getline(cin, phrase);    // Using getline for multi word input, then store in phrase. 

		cout << "Tad: ";
		Tad.respond(phrase);    // Pass the user input to the learner and see if he can respond
	}
}