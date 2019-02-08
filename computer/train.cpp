#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "./auxiliary/duplex.h"
#include "./auxiliary/ml.h"
/*
    Used to test HMM's and potentially neural network
    TODO: Feed a direct line to the lcdk by copying from main.cpp on reading and writing text files
    TODO: This should only be used for the computer program to print out identified words
          Hence, there is no chatbot involved here
*/

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds

// TODO: When project is finalized, run C++ program on WSL and web server on PowerShell
// NOTE: WSL use regular file path
// NOTE: Windows PowerShell use Windows file path
const string LCDK_FILE_PATH = "/mnt/c/Users/ivanm/workspace_v8/lcdk/Debug/";
//const string LCDK_FILE_PATH = "C:\\Users\\ivanm\\workspace_v8\\lcdk\\Debug\\";
const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";
//const string = "C:\\therapist\\computer\\output\\";

const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";
const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";
const string COMPUTER_BUSY_FILE = COMPUTER_OUTPUT_PATH + "busy.txt";


bool conversate(const string INPUT_FILE, const string COMPUTER_BUSY_FILE) {

    // Computer is in busy state
    // TODO: Implement busy.txt file with single character "0"
    //writeBusyFile(BUSY_FILE_NAME, "1");

    // Read the file with MFCC data from the LCDK
    vector<string> inputs = readFile(INPUT_FILE);

    // Feed into HMM
    // TODO: This function gets written in hmm.h
    string client_message = ml(inputs);

    cout << "PROGRAM: " << client_message << endl;

    // Computer is not in busy state
    // TODO: Update busy.txt file with single character "1"
    //writeBusyFile(BUSY_FILE_NAME, "0");

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
                bool finishConversation = conversate(INPUT_FILE, COMPUTER_BUSY_FILE);

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
    // Comment this function out if you do not want to read from the LCDK
    readLCDK();

    // TODO: Create function that runs the training algorithm
}