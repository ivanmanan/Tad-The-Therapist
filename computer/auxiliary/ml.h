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
	void printMFCCs() {
		for(auto it = m_MFCCs.begin(); it != m_MFCCs.end(); it++) {
			cout << *it << " ";
		}
		//cout << " size: " << m_MFCCs.size();
		cout << endl;
	};
	void printStDev() {
		for(auto it = m_StDev.begin(); it != m_StDev.end(); it++) {
			cout << *it << " ";
		}
		//cout << " size: " << m_StDev.size();
		cout << endl;
	};

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
		result *= (2000000000000000 / (root2pi * m_StDev[i])) * exp((-1) * distSquare[i] / (2 * m_StDev[i] * m_StDev[i]));
	}

	return result;
}



///////////////////////////////////////////////////////////////


class HMM {
public:
	HMM(string word, vector<State> states, vector<vector<double>> transProb);

	// Returns probability that the input was produced by the HMM
	double prob(const vector<vector<double>>& input) const;

	// Debug functions
	void printTransProb();
	void printStates();

	// Returns the word of the HMM
	string word() const {return m_word;};

private:
	string m_word;
	int m_numStates;
	vector<State> m_states;
	vector<vector<double>> m_transProb;

	double getAlpha(const vector<vector<double>>& input, const int& tailIdx, const int& stateIdx, vector<vector<double>>& alphaVals) const;
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
	vector<vector<double>> alphaVals;

	//initialize alphaVals to matrix of -1
	vector<double> init(m_numStates, -1);
	for (int i = 0; i < input.size(); i++)
	{
		alphaVals.push_back(init);
	}

	for (int stateIdx = 0; stateIdx < m_numStates; ++stateIdx)
	{
		p += getAlpha(input, tailIdx, stateIdx, alphaVals);
	}

	return p;
}

//recursive function to calculate alpha in forward algorithm
double HMM::getAlpha(const vector<vector<double>>& input, const int& tailIdx, const int& stateIdx, vector<vector<double>>& alphaVals) const
{
	double alpha = 0;

		//base case: if the input size is 1, then alpha is equal to the probability that the 
	//output corresponds to the initial state 
	if (tailIdx == 0)
	{
		if (stateIdx == 0) {
			if (alphaVals[0][0] >= 0)
				return alphaVals[0][0];
			else
				return m_states[0].gaussProb(input[0]);
		}
		//if not initial state, probability is zero
		else
			return 0;
	}

	for (int i = 0; i < m_numStates; ++i)
	{
		if (alphaVals[tailIdx - 1][i] >= 0)
			alpha += alphaVals[tailIdx - 1][i] * m_transProb[i][stateIdx];
		else
			alpha += getAlpha(input, tailIdx - 1, i, alphaVals) * m_transProb[i][stateIdx];
	}

	alpha *= m_states[stateIdx].gaussProb(input[tailIdx]);
	alphaVals[tailIdx][stateIdx] = alpha;
	return alpha;
}


void HMM::printTransProb() {
	cout << "PROGRAM: Printing transitional probability for: " << m_word << endl;
	for(int r = 0; r < m_transProb.size(); r++) {
		for(int c = 0; c < m_transProb[0].size(); c++) {
			cout << m_transProb[r][c] << " ";
		}
		cout << endl;
	}
}

void HMM::printStates() {
	cout << "PROGRAM: Printing states for word: " << m_word << endl;
	for(auto s = m_states.begin(); s != m_states.end(); s++) {
		int state_idx = distance(m_states.begin(), s);
		cout << "Printing for state " << state_idx << endl;
		cout << "Means: ";
		s->printMFCCs();
		cout << "Std Devs: ";
		s->printStDev();
	}
}




#endif