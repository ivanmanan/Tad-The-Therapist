#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <iterator>
#include "./auxiliary/duplex.h"
#include "./auxiliary/ml.h"

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds

// NOTE: When project is finalized, run C++ program on WSL and web server on PowerShell
const string LCDK_FILE_PATH = "/mnt/c/Users/ivanm/workspace_v8/lcdk/Debug/";
const string COMPUTER_WORDS_PATH = "/mnt/c/therapist/computer/words/";
const string COMPUTER_TRANS_PROB_PATH = "/mnt/c/therapist/computer/transProb/";
const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";

const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";
const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";

vector<HMM> hmms;

// TODO: Copy and paste this function into main.cpp
void buildHMMs() {

    string mfccs1 = "1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.1 11.11 12.12 13.13";
	string stdevs1 = "1 1 1 1 1 1 1 1 1 1 1 1 1";
    State s1(mfccs1, stdevs1);
    cout << "IT HAS BEEN DONE!" << endl;

    // NOTE: Must update this array with every known vocabulary word
    vector<string> words = {"test"};
    
    // TODO: Build HMM's here and push into hmms vector
	// Read text files for data --- the HMM's should be reading from the text file
    // Read transitional probability matrix from text file

    for(auto wordPtr = words.begin(); wordPtr != words.end(); wordPtr++) {
        string word = *wordPtr;
        string wordStateFile = COMPUTER_WORDS_PATH + word + ".txt";
        string wordTransProbFile = COMPUTER_TRANS_PROB_PATH + word + ".txt";

        // Read the text files and store as data structures
        vector<State> states;
        vector<vector<double>> transProb;

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
        vector<double> singleRowData;
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
                    singleRowData.push_back(value);
                }
                transProb.push_back(singleRowData);
                singleRowData.clear();
            }
        }

        // Feed state objects into HMM
        HMM hmm(word, states, transProb);

        // DEBUG: Print out states and transition probability
        /*
        cout << "WORD: " << word << endl;
        for(int r = 0; r < transProb.size(); r++) {
            for(int c = 0; c < transProb[0].size(); c++) {
                cout << transProb[r][c] << " ";
            }
            cout << endl;
        }
        */

        hmms.push_back(hmm);
    }
    cout << "PROGRAM: Finished building Hidden Markov Models." << endl;

}

// INPUT: MFCC data text file
// OUTPUT: Recognized word
string ml(string computer_input) {

	// TODO: Run trial code here
	


	return "hello";
}


bool conversate(const string INPUT_FILE) {
    
    // Feed into HMM
    // TODO: This function gets written in hmm.h
    // Give this function the file name
    string client_message = ml(INPUT_FILE);

    cout << "PROGRAM: " << client_message << endl;

    if(client_message != "goodbye") {
        return true;
    }
    else {
        return false;
    }
}

void readLCDK() {

    cout << "PROGRAM: Waiting for LCDK to trigger therapy session..." << endl;

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

        // NOTE: Variables used to detect changes made by the LCDK
        bool previousChange = readCharFile(DONE_FILE, false);
        bool currentChange = readCharFile(DONE_FILE, false);
        while(1) {
            
            currentChange = readCharFile(DONE_FILE, currentChange);

            if (currentChange != previousChange) {
                previousChange = currentChange;
                cout << "PROGRAM: LCDK created new inputs." << endl;
                
                // Read new MFCC inputs
                bool finishConversation = conversate(INPUT_FILE);

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

int main() {
    buildHMMs();
    readLCDK();
}