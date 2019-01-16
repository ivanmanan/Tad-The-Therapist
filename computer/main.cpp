/*
    Main code that goes onto the computer
    Execute this to run trials from the LCDK output
    This code would be running in background continuously, detecting changes in lcdk/output
    It reads the text file made from train.cpp and apply HMM's onto the MFCC's from the LCDK
    This file exports speech recognition and an appropriate response as output/test.txt

    NOTE: I need to implement class objects and place it onto the MakeFile as well
    NOTE: I need to make the C++ file paths and the Makefile compatible with Windows Powershell
    NOTE: May need to create another C++ file 
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;
using namespace this_thread; // sleep_for, sleep_until
using namespace chrono; // nanoseconds, system_clock, seconds

// TODO: When project is finalized, run C++ program on WSL and web server on PowerShell
// NOTE: WSL use regular file path
// NOTE: Windows PowerShell use Windows file path
const string LCDK_FILE_PATH = "/mnt/c/therapist/lcdk/";
//const string LCDK_FILE_PATH = "C:\\therapist\\lcdk\\";

const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";


// Auxilliary function that reads files
vector<string> readFile(const string FILE_NAME) {
    // NOTE: If text file contains a single character, then access it via lines[0]
    vector<string> lines;
    string new_line;
    ifstream myfile(FILE_NAME);

    if (myfile.is_open()) {
        while(getline(myfile, new_line)) {
            lines.push_back(new_line);
        }
        myfile.close();
    }
    // If file does not exist, do nothing

    return lines;
}

// Function that reads start.txt file from the LCDK
string readStartFile() {
    vector<string> start_line = readFile(START_FILE);
    // If read file fails, return 0
    if (start_line.empty()) {
        return "0";
    }
    else {
        return start_line[0];
    }
}


int main() {


    bool changeAlert = false;

    // Read lcdk/start.txt if user triggers a therapy session
    string start = "0";
    while (1) {
        start = readStartFile();
        if (start == "1") {
            break;
        }
        sleep_for(seconds(1));
    }

    cout << "YAY I GOT OUT OF THE WHILE LOOP WOOHOO!" << endl;

    // Interval read file every 2 seconds
    // NOTE: Use delay only for printing text; remove delay for final project
    // TODO: Have a file-watch that detects a binary value
    // TODO: 
    while (1) {
        cout << "Hello world!" << endl;
        sleep_for(seconds(5));
    }


    cout << "Program stopped." << endl;

}