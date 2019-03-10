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

vector<vector<vector<double>>> kmeans(vector<vector<vector<double>>> data) {

    vector<vector<vector<double>>> clusters;

    for(int f = 0; f < data.size(); f++) {
        cout << "File #" << (f+1) << endl;
        for(int r = 0; r < data[f].size(); r++) {
            for(int c = 0; c < data[f][r].size(); c++) {
                cout << data[f][r][c] << " ";
            }
            cout << endl;
        }
    }

    return clusters;
}



void train(string word, vector<string> files) {

    vector<vector<vector<double>>> data;
    vector<vector<double>> singleFileData;
    vector<double> singleRowData;

    double max = -99999999;
    double min =  99999999;

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

                    if (value > max) {
                        max = value;
                    }
                    if (value < min) {
                        min = value;
                    }
                }



                singleFileData.push_back(singleRowData);
                singleRowData.clear();
            }
        }
        data.push_back(singleFileData);
        singleFileData.clear();
    }

    // Add time dimension onto the data set
    double timeInc = (max - min) / (double)data[0].size();
    for(int f = 0; f < data.size(); f++) {
        for (int r = 0; r < data[f].size(); r++) {
            double timeValue = double(r) * timeInc;
            data[f][r].push_back(timeValue);
        }
    }

    // All file data has been loaded onto the 3D array vector<vector<vector<double>>> data;
    /*
    for(int f = 0; f < data.size(); f++) {
        cout << "File #" << (f+1) << endl;
        for(int r = 0; r < data[f].size(); r++) {
            for(int c = 0; c < data[f][r].size(); c++) {
                cout << data[f][r][c] << " ";
            }
            cout << endl;
        }
    }
    */

    // Run the K-means algorithm
    vector<vector<vector<double>>> clusters = kmeans(data);
    
    // Export training set as JSON
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