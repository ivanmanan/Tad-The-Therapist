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




class State
{
public:
	State(string MFCCs, string StDev);
	~State();
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
State::State(string MFCCs, string StDev) {
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

State::~State() {}



vector<double> State::squareDist(const vector<double>& input) const
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
double State::gaussProb(const vector<double>& input) const
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
	HMM(string word, vector<State> states, vector<vector<double>> transProb);

	//returns probability that the input was produced by the HMM
	double prob(const vector<vector<double>>& input) const;

private:
	string m_word;
	int m_numStates;
	vector<State> m_states;
	vector<vector<double>> m_transProb;

	double getAlpha(const vector<vector<double>>& input, const int& tailIdx, const int& stateIdx) const;
};

HMM::HMM(string word, vector<State> states, vector<vector<double>> transProb) {
	m_word = word;
	m_states = states;
	m_transProb = transProb;
	m_numStates = transProb.size();

	//TODO: constructor retrieves expected MFCC vectors and 
	//      transition probability matrix from a file
}


//uses forward algorithm to compute probability of the sequence of MFCCs given the HMM for m_phoneme
double HMM::prob(const vector<vector<double>>& input) const {
	double p = 0;
	int tailIdx = input.size() - 1; //vector index of the last entry

	for (int stateIdx = 0; stateIdx < m_numStates; ++stateIdx)
	{
		p += getAlpha(input, tailIdx, stateIdx);
	}

	return p;
}

//recursive function to calculate alpha in forward algorithm
double HMM::getAlpha(const vector<vector<double>>& input, const int& tailIdx, const int& stateIdx) const
{
	double alpha = 0;

	//base case: if the input size is 1, then alpha is equal to the probability that the 
	//output corresponds to the initial state 
	if (tailIdx == 0)
	{
		if (stateIdx == 0)
			return m_states[0].gaussProb(input[0]);
		//if not initial state, probability is zero
		else
			return 0;
	}

	for (int i = 0; i < m_numStates; ++i)
	{
		alpha += getAlpha(input, tailIdx - 1, i) * m_transProb[i][stateIdx];
	}

	alpha *= m_states[stateIdx].gaussProb(input[tailIdx]);
	return alpha;
}



vector<double> calcStDev(const vector<vector<double>>& clusterMFCCs, const vector<double>& meanMFCCs)
{
	vector<double> stDev(NUM_MFCCS, 0);
	double clusterSize = clusterMFCCs.size();

	for (int mfccIdx = 0; mfccIdx < NUM_MFCCS; ++mfccIdx)
	{
		double var = 0; //variance = stdev^2

		for (int clusterIdx = 0; clusterIdx < clusterMFCCs.size(); ++clusterIdx)
		{
			var += (clusterMFCCs[clusterIdx][mfccIdx] - meanMFCCs[mfccIdx]) * (clusterMFCCs[clusterIdx][mfccIdx] - meanMFCCs[mfccIdx]);
		}

		var /= clusterSize;
		stDev[mfccIdx] = sqrt(var);
	}
	
	return stDev;
}


vector<vector<double>> convertToNumbers(string computer_input) {
	vector<vector<double>> mfccs;
	// TODO: Convert strings to doubles
	return mfccs;
}

/////////////////////////////////////////////////////////////////
// Function that get executed from trial.cpp and main.cpp file

// INPUT: MFCC data text file
// OUTPUT: Recognized word
string ml(string computer_input) {

	// TODO: Run trial code here
	vector<vector<double>> mfccs = convertToNumbers(computer_input);
	

	// TODO: Build HMM's here

	return "hello";
}




#endif