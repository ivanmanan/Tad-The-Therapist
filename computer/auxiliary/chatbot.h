#ifndef CHATBOT_H
#define CHATBOT_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;

struct probResponse {
	int probability;
	string response;
};

unordered_map<string, vector<probResponse>> preprocessing() {
	// Maintain vocabulary as global variable
	// Key: vocabulary word; Value: vector with probabilities and responses
	unordered_map<string, vector<probResponse>> vocabulary_map;

	ifstream vocabulary("./auxiliary/vocabulary.txt");
	string clientWord;
	string response;
	string temp;
	if(vocabulary.is_open()) {
		while(!vocabulary.eof()) {
			getline(vocabulary, clientWord);
			string key = clientWord;
			getline(vocabulary, response);
			vector<int> probabilities;
			vector<string> responses;
			vector<probResponse> probResponses;
			int probability = 0;
			// Parse the string
			for(int i = 0, j = 0; i < response.size(); i++) {
				if(response[i] == ';') {
					j++;
					string res = response.substr(j, i-j);
					responses.push_back(res);
					j = i;
				}
				// Assume any number found is a probabilty that is always 2 digits
				else if(isdigit(response[i])) {
					string probabilityString = "";
					probabilityString.push_back(response[i]);
					probabilityString.push_back(response[++i]);
					probability += stoi(probabilityString);
					// NOTE: 00 means 100
					if (probability == 0) {
						probability = 100;
					}
					probabilities.push_back(probability);
					j = i;
				}
			}
			auto ite = responses.begin();
			for(auto it = probabilities.begin(); it != probabilities.end(); it++, ite++) {
				probResponse insertProbResponse;
				insertProbResponse.probability = *it;
				insertProbResponse.response = *ite;
				probResponses.push_back(insertProbResponse);
			}
			vocabulary_map.insert({key,probResponses});
		}
		vocabulary.close();
	}
	else {
		cout << "ERROR: Vocabulary file was not found!" << endl;
	}
	return vocabulary_map;
}

void readVocabularyMap(const unordered_map<string, vector<probResponse>>& vocabulary_map) {
	for(auto it = vocabulary_map.begin(); it != vocabulary_map.end(); it++) {
		cout << "Client message: " << it->first << endl;
		cout << "Possible Responses: " << endl;
		// Print out probabilities and responses
		for(auto ite = (it->second).begin(); ite != (it->second).end(); ite++) {
			probResponse value = *ite;
			cout << "Probability Value " << value.probability << ": ";
			cout << value.response << endl;
		}
		cout << "==============================" << endl;
	}
	if (vocabulary_map.empty()) {
		cout << "Vocabulary Map is empty!" << endl;
	}
}




/*
	The following function will look for keywords in the vocabulary file.
	If there is a match, the accompanying response, stored below the initial phrase,
	will be outputed.
	If the response cannot be found, the learner will repeat the phrase, and prompt
	the user to enter an ideal response. This response will be stored in the vocabulary
	file along with the initial phrase.
*/

string respond(const string phrase, const unordered_map<string, vector<probResponse>>& vocabulary_map) {
	// NOTE: Use this function for reading the hash table
	//readVocabularyMap(vocabulary_map);

	auto got = vocabulary_map.find(phrase);
	if(got != vocabulary_map.end()) {
		// Iterate through possible responses and select from probabilistic distribution
		srand (time(NULL));
		int probability_value = rand() % 100;
		cout << probability_value << endl;
		vector<int> probability_distribution;
		// Find probability in hash table closes to probability value
		for(auto it = (got->second).begin(); it != (got->second).end(); it++) {
			probability_distribution.push_back(it->probability);
		}
		auto const res = std::lower_bound(probability_distribution.begin(), probability_distribution.end(), probability_value);
	    if(res == probability_distribution.end()) {
			return "I did not quite get that. Can you say that again?";
		}
		else {
			for(auto it = (got->second).begin(); it != (got->second).end(); it++) {
				if(*res == it->probability) {
					return it->response;
				}
			}
		}
	}
	// If word was not found in the hash table, then output default response
	int probability_value = rand() % 100;
	if(probability_value > 75) {
		return "How does that make you feeeeeeel?";
	}
	else {
		return "I did not quite get that. Can you say that again?";
	}
}

#endif