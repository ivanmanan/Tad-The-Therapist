#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "./auxiliary/duplex.h"
#include "./auxiliary/ml.h"

using namespace std;

// NOTE: When project is finalized, run C++ program on WSL and web server on PowerShell
const string COMPUTER_OUTPUT_PATH = "/mnt/c/therapist/computer/output/";

/*
const string START_FILE = LCDK_FILE_PATH + "start.txt";
const string INPUT_FILE = LCDK_FILE_PATH + "input.txt";
const string DONE_FILE  = LCDK_FILE_PATH + "done.txt";
*/

// TODO: Create multiple files and a file path for training data

const string COMPUTER_OUTPUT_FILE = COMPUTER_OUTPUT_PATH + "output.txt";


int main() {
    cout << "Hello!" << endl;

}