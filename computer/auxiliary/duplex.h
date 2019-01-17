#ifndef DUPLEX_H
#define DUPLEX_H

#include <fstream>
#include <vector>
#include <string>

using namespace std;

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

// Auxiliary function that reads files
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


// Auxiliary that writes onto files
// NOTE: If file does not exist already, then this function creates that file
// NOTE: When appending to file the 2nd dialogue, it must start with newline insertion
void appendToFile(const string FILE_NAME, const string message) {
    ofstream myfile(FILE_NAME, fstream::app);
    string new_line = message;
    myfile << new_line;
    myfile.close();
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


// Function that deletes current output.txt file and creates a new one with a greeting
void startOutputFile() {
    // Delete existing output.txt file
    ofstream ofs;
    ofs.open(COMPUTER_OUTPUT_FILE, ofstream::trunc);
    ofs.close();

    // Append greeting message to new output.txt
    string message = "Hello. My name is Tad. How can I help you?";
    appendToFile(COMPUTER_OUTPUT_FILE, message);
}

#endif