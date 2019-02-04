#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include "./auxiliary/chatbot.h"

/*
	Given an input, identify key words and output an appropriate response


	"not sad" is first phrase to identify, in contrast to "sad"
*/

using namespace std;

//unordered_map<string,string> preprocessing() {
// TODO: I can do parallel programming to make this faster if text file gets too big
void preprocessing() {

	// Create multimap; obtain list with strings and probability; store into another list
	multimap<char,int> mymm;

	mymm.insert (std::make_pair('x',10));
	mymm.insert (std::make_pair('y',20));
	mymm.insert (std::make_pair('z',30));
	mymm.insert (std::make_pair('z',40));

	// Prints all elements in linked list of hash table
	auto range = mymm.equal_range('z');
	for (auto it = range.first; it != range.second; ++it){
		cout << it->second << endl;
	}

}

int main() {

	preprocessing();

	cout << "Hello my name is Tad, how can I help you?\n";
	
	// TODO: Pass string here that breaks out of the infinite loop
	// 		 e.g. when user says goodbye
	while(1) {

		// Store client response in phrase
		cout << "Client: ";
		string phrase;
		getline(cin, phrase);

		// Pass the user input to the learner and see if he can respond
		cout << "Tad: ";
		respond(phrase);
	}
}