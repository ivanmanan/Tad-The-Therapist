#include "./auxiliary/chatbot.h"
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
	unordered_map<string, vector<probResponse>> vocabulary_map = preprocessing();
	cout << "Tad: Hello my name is Tad, how can I help you?\n";
	
	while(1) {
		// Store client response in phrase
		cout << "Client: ";
		string phrase = "sad";
		getline(cin, phrase);

		// Program responds to client message
		string response = respond(phrase, vocabulary_map);
		cout << "Tad: " << response << endl;

		// End program if client says goodbye
		if(phrase == "goodbye") {
			break;
		}
	}
}