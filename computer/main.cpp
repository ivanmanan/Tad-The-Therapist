/*
    Main code that goes onto the computer
    Execute this to run trials from the LCDK output
    This code would be running in background continuously, detecting changes in lcdk/output
    It reads the text file made from train.cpp and apply HMM's onto the MFCC's from the LCDK
    This file exports speech recognition and an appropriate response as output/test.txt

    NOTE: I need to implement class objects and place it onto the MakeFile as well
    NOTE: I need to make the C++ file paths and the Makefile compatible with Windows Powershell
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <fstream>
#include <sstream>
#include <iterator>
#include "./auxiliary/duplex.h"
#include "./auxiliary/conversation.h"
#include "./auxiliary/chatbot.h"
#include "./auxiliary/ml.h"

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds


// TODO: When project is finalized, run C++ program on WSL and web server on PowerShell
// NOTE: WSL use regular file path
// NOTE: Windows PowerShell use Windows file path
const string LCDK_FILE_PATH = "/mnt/c/Users/ivanm/workspace_v8/lcdk/Debug/";
const string COMPUTER_WORDS_PATH = "/mnt/c/therapist/computer/words/";
const string COMPUTER_TRANS_PROB_PATH = "/mnt/c/therapist/computer/transProb/";
const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";

const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";
const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";

vector<HMM> hmms;

void buildHMMs() {

    // NOTE: Must update this array with every known vocabulary word
    vector<string> words = {"cat", "dog", "professor"};
    
	// Read text files for data to insert into HMM
    for(auto wordPtr = words.begin(); wordPtr != words.end(); wordPtr++) {
        string word = *wordPtr;
        string wordStateFile = COMPUTER_WORDS_PATH + word + ".txt";
        string wordTransProbFile = COMPUTER_TRANS_PROB_PATH + word + ".txt";

        // Read the text files and store as data structures
        vector<State> states;
        vector<vector<long double>> transProb;
        string mfccData;
        string stdDevData;
        ifstream stateFile(wordStateFile);

        if(stateFile.is_open()) {
            // Read MFCC values
            while(getline(stateFile, mfccData)) {
                // Read std deviations
                getline(stateFile, stdDevData);

                // NOTE: State expects 13 values in each vector argument
                State state(mfccData, stdDevData);
                states.push_back(state);
            }
        }

        // NOTE: transProb must be converted from string to double
        string prob_line;
        ifstream transProbFile(wordTransProbFile);
        vector<long double> singleRowData;
        if(transProbFile.is_open()) {
            while(getline(transProbFile, prob_line)) {
                stringstream ss(prob_line);
                
                istream_iterator<string> begin(ss);
                istream_iterator<string> end;
                vector<string> vstrings(begin, end);

                // Convert string to double
                for(auto valuePtr = vstrings.begin(); valuePtr != vstrings.end(); valuePtr++) {
                    string valueString = *valuePtr;
                    double value;

                    stringstream(valueString) >> value;
                    long double lvalue = (long double) value;
                    singleRowData.push_back(lvalue);
                }
                transProb.push_back(singleRowData);
                singleRowData.clear();
            }
        }

        // Feed state objects into HMM
        HMM hmm(word, states, transProb);

        // DEBUG: Print out states and transition probability
        //hmm.printTransProb();
        //hmm.printStates();

        hmms.push_back(hmm);
    }
    cout << "PROGRAM: Finished building Hidden Markov Models." << endl;

}

// INPUT: MFCC data text file
// OUTPUT: Recognized word
string ml(string computer_input) {

    // Take input file and convert to matrix of doubles
    vector<vector<long double>> input;
    vector<long double> singleRowInput;
    string input_line;
    ifstream inputFile(computer_input);
    if(inputFile.is_open()) {
        while(getline(inputFile, input_line)) {
            stringstream ss(input_line);
                
            istream_iterator<string> begin(ss);
            istream_iterator<string> end;
            vector<string> vstrings(begin, end);

            // Convert string to double
            for(auto valuePtr = vstrings.begin(); valuePtr != vstrings.end(); valuePtr++) {
                string valueString = *valuePtr;
                double value;

                stringstream(valueString) >> value;
                long double lvalue = (long double)value;
                singleRowInput.push_back(lvalue);
            }
            input.push_back(singleRowInput);
            singleRowInput.clear();
        }
    }

    // DEBUG: Printing out input matrix
    /*
    cout << "DEBUG: Input matrix from the LCDK: " << endl;
    for(int r = 0; r < input.size(); r++) {
        for(int c = 0; c < input[0].size(); c++) {
            cout << input[r][c] << " ";
        }
        cout << endl;
    }
    */


    double max_probability = 0;
    string likely_word;

    // TODO: There is an error with hmm->prob or the input 2d array due to scaling
    // Multiply scale by a factor of 10
    //const long double scale = 100;
    


	// Test the mfcc input to every HMM and return the word with greatest probability
    long double scale = 1;

    cout << "==========================================================" << endl;
    while(1) {

        if(max_probability > 0) {
            break;
        }

        for(auto hmm = hmms.begin(); hmm != hmms.end(); hmm++) {
            string word = hmm->word();

            // TODO: Must create for-loop that scales until i get a probability
            //       Must be able to break out of infinite recursion

            double probability = hmm->prob(input, scale);
            if(probability > max_probability) {
                max_probability = probability;
                likely_word = word;
            }
        }
        scale = scale * 5;
    }
	return likely_word;
}


bool conversate(const string INPUT_FILE, unordered_map<string, vector<probResponse>>& vocabulary) {
    
    // Feed into HMM
    string client_message = ml(INPUT_FILE);
    cout << "USER: " << client_message << endl;

    // Obtain resposne from Tad
    string response = respond(client_message, vocabulary);
    cout << "PROGRAM: " << response << endl;

    // Append to output file for web server to read
    appendToFile(COMPUTER_OUTPUT_FILE, "\n" + client_message);
	appendToFile(COMPUTER_OUTPUT_FILE, "\n" + response);

    if(client_message != "goodbye") {
        return false;
    }
    else {
        return true;
    }
}


int main() {
    buildHMMs();
    unordered_map<string, vector<probResponse>> vocabulary = preprocessing();


    while(1) {
        // Read lcdk/start.txt if user triggers a therapy session
        bool previousStart = readCharFile(START_FILE, false);
        bool currentStart = readCharFile(START_FILE, false);
        while(1) {
            currentStart = readCharFile(START_FILE, currentStart);
            if (currentStart != previousStart) {
                break;
            }
            // NOTE: Read file every 2 second intervals
            // NOTE: Use delay only for printing text; remove delay for final project
            sleep_for(seconds(1));
        }

        cout << "\nPROGRAM: User initialized therapy session..." << endl;

        // Initialize the computer/output/output.txt file
        startOutputFile(COMPUTER_OUTPUT_FILE);

        cout << "PROGRAM: Tad sent out the first message." << endl;

        // NOTE: Variables used to detect changes made by the LCDK
        bool previousChange = readCharFile(DONE_FILE, false);
        bool currentChange = readCharFile(DONE_FILE, false);
        while(1) {
            
            currentChange = readCharFile(DONE_FILE, currentChange);

            if (currentChange != previousChange) {
                previousChange = currentChange;
                cout << "PROGRAM: LCDK created new inputs." << endl;
                // Read MFCC inputs
                bool finishConversation = conversate(INPUT_FILE, vocabulary);
                // NOTE: When conversation is over, break from while-loop
                //       and restart conversation again
                if (finishConversation) {
                    break;
                }
            }
            // NOTE: Read file every 2 second intervals
            // NOTE: Use delay only for printing text; remove delay for final project
            sleep_for(seconds(1));
        }
        cout << "PROGRAM: Conversation finished. Listening for new conversation..." << endl;
    }
}