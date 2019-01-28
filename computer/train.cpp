#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "./auxiliary/duplex.h"
/*
    Used to test HMM's and potentially neural network
    TODO: Feed a direct line to the lcdk by copying from main.cpp on reading and writing text files
    TODO: This should only be used for the computer program to print out identified words
          Hence, there is no chatbot involved here
*/

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds


void readLCDK() {

    /*
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
                // Read new neural network inputs
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
    */
}

int main() {
    // Comment this function out if you do not want to read from the LCDK
    readLCDK();
}