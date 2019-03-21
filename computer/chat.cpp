#include "./auxiliary/chatbot.h"
#include "./auxiliary/duplex.h"
#include <unordered_map>
#include <vector>

const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";
const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";


using namespace std;

int main() {
	unordered_map<string, vector<probResponse>> vocabulary_map = preprocessing();
	string greeting = "Tad: Hello my name is Tad, how can I help you?";
	cout << greeting << endl;
	startOutputFile(COMPUTER_OUTPUT_FILE);
	
	while(1) {
		// Store client response in phrase
		cout << "Client: ";
		string phrase = "sad";
		getline(cin, phrase);

		// Program responds to client message
		string response = respond(phrase, vocabulary_map);
		cout << "Tad: " << response << endl;

		// Output to output.txt
		appendToFile(COMPUTER_OUTPUT_FILE, "\n" + phrase);
		appendToFile(COMPUTER_OUTPUT_FILE, "\n" + response);

		// End program if client says goodbye
		if(phrase == "goodbye") {
			break;
		}
	}
}