#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include "./auxiliary/duplex.h"
#include "./auxiliary/ml.h"
using namespace std;

const string COMPUTER_TRAINING_PATH = "/mnt/c/therapist/computer/training/";

// Training set for the word "cat"
const string CAT1 = COMPUTER_TRAINING_PATH + "cat1.txt";
const string CAT2 = COMPUTER_TRAINING_PATH + "cat2.txt";
const string CAT3 = COMPUTER_TRAINING_PATH + "cat3.txt";
const string CAT4 = COMPUTER_TRAINING_PATH + "cat4.txt";
const string CAT5 = COMPUTER_TRAINING_PATH + "cat5.txt";

void train(string word, vector<string> files) {

    vector<vector<vector<double>>> data;
    vector<vector<double>> singleFileData;
    vector<double> singleRowData;

    // Read each file and store into array of files with all its MFCC's
    for(auto filePtr = files.begin(); filePtr != files.end(); filePtr++) {
        int fileIndex = distance(files.begin(), filePtr);
        string file = *filePtr;
        //cout << file << " Index: " << fileIndex << endl;
        string mfcc_line;
        ifstream fileData(file);
        if(fileData.is_open()) {
            int row = 0;
            while(getline(fileData, mfcc_line)) {
                stringstream ss(mfcc_line);
                
                istream_iterator<string> begin(ss);
                istream_iterator<string> end;
                vector<string> vstrings(begin, end);

                // Convert string to double
                for(auto valuePtr = vstrings.begin(); valuePtr != vstrings.end(); valuePtr++) {
                    int mfccIndex = distance(vstrings.begin(), valuePtr);
                    string valueString = *valuePtr;
                    double value;
                    stringstream(valueString) >> value;
                    singleRowData.push_back(value);
                }
                singleFileData.push_back(singleRowData);
                singleRowData.clear();
            }
        }
        data.push_back(singleFileData);
        singleFileData.clear();
    }


    // All file data has been loaded onto the 3D array vector<vector<vector<double>>> data;
    for(int f = 0; f < data.size(); f++) {
        cout << "File #" << (f+1) << endl;
        for(int r = 0; r < data[f].size(); r++) {
            for(int c = 0; c < data[f][r].size(); c++) {
                cout << data[f][r][c] << " ";
            }
            cout << endl;
        }
    }
}

int main() {

    vector<string> cat = {CAT1, CAT2, CAT3, CAT4, CAT5};

    // Add words to train
    vector<string> words = {"cat"};
    vector<vector<string>> files = {cat};

    auto file = files.begin();
    for(auto word = words.begin(); word != words.end(); word++, file++) {
        train(*word, *file);
    }
}