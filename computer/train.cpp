#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
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

void train(string word) {

    vector<string> files = {CAT1, CAT2, CAT3, CAT4, CAT5};

    for(auto it = files.begin(); it != files.end(); it++) {
        cout << *it << endl;
    }

}

int main() {
    // Enter string to train
    train("cat");
}