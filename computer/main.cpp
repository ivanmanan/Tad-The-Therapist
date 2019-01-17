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

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds


// TODO: When project is finalized, run C++ program on WSL and web server on PowerShell
// NOTE: WSL use regular file path
// NOTE: Windows PowerShell use Windows file path
const string LCDK_FILE_PATH = "/mnt/c/therapist/lcdk/";
//const string LCDK_FILE_PATH = "C:\\therapist\\lcdk\\";
const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";
//const string = "C:\\therapist\\computer\\output\\";

const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";
const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";


int main() {

    // Read lcdk/start.txt if user triggers a therapy session
    bool start = false;
    while (1) {
        start = readCharFile(START_FILE);
        if (start) {
            break;
        }
        sleep_for(seconds(1));
    }

    cout << "PROGRAM: User initialized therapy session." << endl;

    // Initialize the computer/output/output.txt file
    startOutputFile(COMPUTER_OUTPUT_FILE);

    cout << "PROGRAM: Tad sent out the first message." << endl;

    // Interval read file every 2 seconds
    // NOTE: Use delay only for printing text; remove delay for final project
     // NOTE: Variables used to detect changes made by the LCDK
    bool previousChange = false;
    bool currentChange = false;
    while (1) {

        currentChange = readCharFile(DONE_FILE);

        if (currentChange != previousChange) {
            previousChange = currentChange;
            cout << "New file change!" << endl;
            // Read new neural network inputs

        }
        sleep_for(seconds(5));
    }


    cout << "Program stopped." << endl;

}