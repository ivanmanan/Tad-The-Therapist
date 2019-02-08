#ifndef ML_H
#define ML_H

#include <vector>
#include <iostream>
#include <string>

// mfcc is a 13 digit array

vector<double> convertToNumbers(vector<string> computer_input) {
    vector<double> mfccs;

    // TODO: Convert strings to doubles

    return mfccs;
}

string ml(vector<string> computer_input) {

    vector<double> mfccs = convertToNumbers(computer_input);
    return "hello";
}

#endif