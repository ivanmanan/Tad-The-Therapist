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
using namespace std;

vector<int> readTrainingFile() {
    vector<int> temp;
    return temp;
}

vector<int> readLCDK() {
    vector<int> temp;
    cout << "HI!"<< endl;
    return temp;
}

int main() {

    // Read text file
    // I may want to make a vector of class objects
    // I want to make variables here I guess?
    // Can pass paramter and return vectors
    vector<int> training_set = readTrainingFile();
    vector<int> trial_set = readLCDK();

    cout << "Hello world!" << endl;
}