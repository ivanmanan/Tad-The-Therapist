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
#include <thread>
#include "./auxiliary/duplex.h"
#include "./auxiliary/conversation.h"

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


int main() {

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
}