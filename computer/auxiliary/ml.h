#ifndef ML_H
#define ML_H

#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int NUM_MFCCS = 13;
const double pi = 3.14159;
const double root2pi = sqrt(2 * pi);




class MeanMFCC
{
public:
	MeanMFCC(string MFCCs, string StDev);
	~MeanMFCC();
	vector<double> squareDist(const vector<double>& input) const;
	vector<double> getStDev() const;
	double gaussProb(const vector<double>& input) const;

private:
	//pointers to 13 long array of doubles that contain the mean and standard
	//deviation in each dimension
	vector<double> m_StDev;
	vector<double> m_MFCCs;
};

//string contains the 13 MFCC values, each separated by one space
MeanMFCC::MeanMFCC(string MFCCs, string StDev) {
	size_t sz = 0;
	size_t sz_start = 0;

	for (int i = 0; i < NUM_MFCCS; ++i) {
		m_MFCCs.push_back(stod(MFCCs.substr(sz_start), &sz));
		sz_start += sz;
	}

	sz = 0;
	sz_start = 0;

	for (int i = 0; i < NUM_MFCCS; ++i) {
		m_StDev.push_back(stod(StDev.substr(sz_start), &sz));
		sz_start += sz;
	}
}

MeanMFCC::~MeanMFCC() {}



vector<double> MeanMFCC::squareDist(const vector<double>& input) const
{
	vector<double> output;

	//return empty vector if the size is mismatched
	if (input.size() != NUM_MFCCS) {
		cout << "Invalid MFCC array size" << endl;
		return output;
	}

	for (int i = 0; i < NUM_MFCCS; ++i)
	{
		output.push_back((input[i] - m_MFCCs[i]) * (input[i] - m_MFCCs[i]));
	}

	return output;
}


//computes the probability that the input was produced by the state
//that this corresponds to using a gaussian distribution
//to simplify, we assume that the distributions of values in each dimension are independent 
double MeanMFCC::gaussProb(const vector<double>& input) const
{
	vector<double> distSquare = squareDist(input);

	double result = 1;

	for (int i = 0; i < NUM_MFCCS; ++i)
	{
		result *= (1 / (root2pi * m_StDev[i])) * exp((-1) * distSquare[i] / (2 * m_StDev[i] * m_StDev[i]));
	}

	return result;
}



///////////////////////////////////////////////////////////////


class HMM {
public:
	HMM(string word, vector<MeanMFCC> stateOutput, vector<vector<double>> transProb);

	//returns probability that the input was produced by the HMM
	double prob() const;

private:
	string m_word;
	int m_numStates;
	vector<MeanMFCC> m_stateOutput;
	vector<vector<double>> m_transProb;
};

HMM::HMM(string word, vector<MeanMFCC> stateOutput, vector<vector<double>> transProb) {
	m_word = word;
	m_stateOutput = stateOutput;
	m_transProb = transProb;
	m_numStates = transProb.size();

	//TODO: constructor retrieves expected MFCC vectors and 
	//      transition probability matrix from a file
}

double HMM::prob() const {
	double p = 0;

	//TODO: forward algorithm to compute probability of 
	//the sequence of MFCCs given the HMM for m_phoneme

	return p;
}


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