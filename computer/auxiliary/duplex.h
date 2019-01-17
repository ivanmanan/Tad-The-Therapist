#ifndef DUPLEX_H
#define DUPLEX_H

#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Auxiliary function that reads files
vector<string> readFile(const string FILE_NAME) {
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


// Function that reads single character files from the LCDK
bool readCharFile(const string FILE_NAME, bool currentValue) {
    vector<string> start_line = readFile(FILE_NAME);
    
    // If read file fails, return false
    if (start_line.empty()) {
        return currentValue;
    }
    if (start_line[0] == "1") {
        return true;
    }
    if (start_line[0] == "0") {
        return false;
    }
}


// Function that deletes current output.txt file and creates a new one with a greeting
void startOutputFile(const string FILE_NAME) {
    // Delete existing output.txt file
    ofstream ofs;
    ofs.open(FILE_NAME, ofstream::trunc);
    ofs.close();
    // Append greeting message to new output.txt
    string message = "Hello. My name is Tad. How can I help you?";
    appendToFile(FILE_NAME, message);
}

#endif