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


int main() {

    // Read lcdk/start.txt if user triggers a therapy session
    string start = "0";
    while (1) {
        start = readStartFile();
        if (start == "1") {
            break;
        }
        sleep_for(seconds(1));
    }

    cout << "PROGRAM: User initialized therapy session." << endl;

    // Initialize the computer/output/output.txt file
    startOutputFile();

    cout << "PROGRAM: Tad sent out the first message." << endl;

    // Interval read file every 2 seconds
    // NOTE: Use delay only for printing text; remove delay for final project
    // TODO: Have a file-watch that detects a binary value
    while (1) {
        cout << "Hello world!" << endl;
        sleep_for(seconds(5));
    }


    cout << "Program stopped." << endl;

}