#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <time.h>
#include "./auxiliary/duplex.h"
#include "./auxiliary/ml.h"
using namespace std;

const string COMPUTER_TRAINING_PATH = "/mnt/c/therapist/computer/training/";
const string COMPUTER_WORDS_PATH = "/mnt/c/therapist/computer/words/";
const string COMPUTER_TRANS_PROB_PATH = "/mnt/c/therapist/computer/transProb/";

//error threshold for determining convergence of kmeans
const double errorThresholdSquare = 0.000001;


//calculate square of distance between 2 vectors
double sqDist(vector<double> centroid, vector<double> point) {
        if (centroid.size() != point.size())
            return -1;

        double ans = 0;
        for (int i = 0; i < centroid.size(); i++)
        {
            ans += (centroid[i] - point[i]) * (centroid[i] - point[i]);
        }
        return ans;
}

vector<double> calcMean(const vector<vector<double>>& clusterMFCCs)
{
    //returns an empty vector if passed an empty vector
    if (clusterMFCCs.empty()) {
        vector<double> emptyVec;
        return emptyVec;
    }

	vector<double> mean(clusterMFCCs[0].size(), 0);
	double clusterSize = clusterMFCCs.size();

	for (int mfccIdx = 0; mfccIdx < clusterMFCCs[0].size(); ++mfccIdx)
	{
		for (int clusterIdx = 0; clusterIdx < clusterMFCCs.size(); ++clusterIdx)
		{
			mean[mfccIdx] += clusterMFCCs[clusterIdx][mfccIdx];
		}
		mean[mfccIdx] /= clusterSize;
	}

	return mean;
}

vector<double> calcStDev(const vector<vector<double>>& clusterMFCCs, const vector<double>& meanMFCCs)
{
    //returns an empty vector if passed an empty vector
    if (clusterMFCCs.empty() || meanMFCCs.empty()) {
        vector<double> emptyVec;
        return emptyVec;
    }

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

//returns true if v1 should come before v2
//used for sorting the clusters
bool compareVecs(const vector<double>& v1, const vector<double>& v2)
{
    if (v1.empty() && v2.empty())
        return true;
    else if (v1.empty())
        return false;
    else if (v2.empty())
        return true;
    else
    {
        return v1[13] <= v2[13];
    }
    return true;
}


vector<vector<vector<double>>> kmeans(const vector<vector<vector<double>>>& data, int numClusters, double min, double max) {

    vector<vector<vector<double>>> clustersData = data;
    vector<vector<double>> centroids;
    vector<double> newCentroid(14);
    vector<double> error(numClusters, 99999);
    bool keepRunning = true;


    ////////////////////////////
    //initialization
    ////////////////////////////
    srand(time(NULL));
    for (int i = 0; i < numClusters; i++)
    {
        /*
        //calculate random starting centroids
        for (int m = 0; m < 14; m++)
        {
            //confine starting means within the central 10% of the dataset
            double newMax = max - ((max - min) * 0.45);
            double newMin = min + ((max - min) * 0.45);
            double next = (newMax - newMin) * ( (double)rand() / (double)RAND_MAX ) + newMin;
            newCentroid[m] = next;
            //cout << newCentroid[m] << " ";
        }
        centroids.push_back(newCentroid);
        */
       
       int idx = i * data[0].size() / numClusters;
       centroids.push_back(data[0][idx]);

        /*
        for (int m = 0; m < 14; m++)
        {
            cout << centroids[i][m] << " ";
        }
        cout << endl;
        */
    }

    ////////////////////////////
    //iteration
    ////////////////////////////
    while (keepRunning) {
        vector<vector<vector<double>>> nextClustersData(numClusters);
        vector<vector<double>> nextCentroids;

        //assign each vector to a cluster around the closest centroid
        for (int f = 0; f < clustersData.size(); f++) {
            for (int r = 0; r < clustersData[f].size(); r++) {
                double minDist = 99999999;
                int clusterIdx;
                for (int i = 0; i < centroids.size(); i++) {
                    double dist = sqDist(centroids[i], clustersData[f][r]);
                    if (dist < minDist) {
                        minDist = dist;
                        clusterIdx = i;
                    }
                }
                nextClustersData[clusterIdx].push_back(clustersData[f][r]);
            }
        }
        
        for (int f = 0; f < nextClustersData.size(); f++) {
            //cout << nextClustersData[f].size() << endl;
            /*for (int r = 0; r < nextClustersData[f].size(); r++) {
                for (int c = 0; c < nextClustersData[f][r].size(); c++) {
                    cout << nextClustersData[f][r][c] << " ";
                }
                cout << endl;
            }*/
        }

        //set centroids to the averages of the new clusters
        for (int i = 0; i < numClusters; i++) {
            if (!nextClustersData[i].empty()) 
                nextCentroids.push_back(calcMean(nextClustersData[i]));
        }

        //calculate error and check for convergence
        for (int i = 0; i < numClusters; i++) {
            if (sqDist(nextCentroids[i], centroids[i]) > errorThresholdSquare)
                break;

            //checks if every centroid meets the convergence requirement
            if (i == numClusters - 1)
                keepRunning = false;
        }

        //update values for next iteration
        centroids = nextCentroids;
        clustersData = nextClustersData;

        keepRunning = false;
    }

    

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
    return clustersData;
}




void train(string word, vector<string> files, int clusters) {

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
    vector<vector<vector<double>>> clustersData = kmeans(data, clusters, min, max);
    
    //calculate mean of each cluster
    //empty clusters will generate empty mean vectors
    vector<vector<double>> means;
    for (int i = 0; i < clustersData.size(); i++) {
        means.push_back(calcMean(clustersData[i]));
    }

    //calculate standard deviation of each cluster
    //empty clusters will generate empty stDev vectors
    vector<vector<double>> stDevs;
    for (int i = 0; i < clustersData.size(); i++) {
        stDevs.push_back(calcStDev(clustersData[i], means[i]));
    }
  
    //sort means, stDevs, and clustersData based on temporal order
    bool stop = false;
    while (!stop)
    {
        stop = true;
        for (int i = 0; i < means.size() - 1; i++)
        {
            //check means[i] <= means[i+1]
            if (!compareVecs(means[i], means[i+1])) {
                stop = false;
                vector<double> temp = means[i];
                means[i] = means[i+1];
                means[i+1] = temp;
                temp = stDevs[i];
                stDevs[i] = stDevs[i+1];
                stDevs[i+1] = temp;
                vector<vector<double>> temp2 = clustersData[i];
                clustersData[i] = clustersData[i+1];
                clustersData[i+1] = temp2;
            }
        }
    }
 
    //generate transition probability matrix
    vector<vector<double>> transProb;
    int totalSamples = data.size() * data[0].size();
    for (int i = 0; i < clustersData.size(); i++) {
        vector<double> row(clustersData.size());
        if (!clustersData[i].empty()) {
            if (clustersData[i+1].empty() || i == clustersData.size() - 1)
                row[i] = 1;
            else {
                row[i] = ((double)clustersData[i].size() - files.size()) / (double)clustersData[i].size();
                row[i+1] = 1 - row[i];
            }
        }
        transProb.push_back(row);
    }

    /*
    for (int r = 0; r < transProb.size(); r++) {
        for (int c = 0; c < transProb[r].size(); c++){
            cout << transProb[r][c] << " ";
        }
        cout << endl;
    }
    */


    // Export means and stDevs as text file
    // Export transProb as text file
    string word_path = COMPUTER_WORDS_PATH + word + ".txt";
    string transProb_path = COMPUTER_TRANS_PROB_PATH + word + ".txt";
    
    ofstream wordfile(word_path);
    int clusterCtr = clusters;

    // Must alternate between rows
    if(wordfile.is_open()) {

        /*
        // Read entire 2D vector
        cout << means.size() << endl;
        cout << stDevs.size() << endl;

        cout << "PRINTING MEANS: " << endl;
        for(int r = 0; r < means.size(); r++) {
            for(int c = 0; c < means[r].size(); c++) {
                cout << means[r][c] << " ";
            }
            cout << endl;
        }

        cout << "PRINTING STD DEVIATIONS: " << endl;
        for(int r = 0; r < stDevs.size(); r++) {
            for(int c = 0; c < stDevs[r].size(); c++) {
                cout << stDevs[r][c] << " ";
            }
            cout << endl;
        }
        */

        // NOTE: This is shoddy
        for(int r = 0; r < means.size(); r++) {
            for(int c = 0; c < means[r].size(); c++) {
                //cout << means[r].size() - 1<< endl;
                if (c < means[r].size() - 1) {
                    string valueToFile = to_string(means[r][c]) + " ";
                    wordfile << valueToFile;
                }
            }
            if(!means[r].empty()) {
                wordfile << "\n";
            }
            else
            {
                clusterCtr--;
            }
            

            for(int c = 0; c < stDevs[r].size(); c++) {
                //cout << stDevs[r].size() << endl;
                string valueToFile = to_string(stDevs[r][c]) + " ";
                wordfile << valueToFile;
            }
            if(!stDevs[r].empty()) {
                wordfile << "\n";
            }
        }
    }


    ofstream transProbFile(transProb_path);
    if(transProbFile.is_open()) {
        for(int r = 0; r < clusterCtr; r++) {
            for(int c = 0; c < clusterCtr; c++) {
                string valueToFile = to_string(transProb[r][c]) + " ";
                transProbFile << valueToFile;
            }
            transProbFile << "\n";
        }
    }


}

int main() {
    
    // NOTE: Must update known words with proper parameters

    // Training set for the word "cat"
    const string CAT1 = COMPUTER_TRAINING_PATH + "cat1.txt";
    const string CAT2 = COMPUTER_TRAINING_PATH + "cat2.txt";
    const string CAT3 = COMPUTER_TRAINING_PATH + "cat3.txt";
    const string CAT4 = COMPUTER_TRAINING_PATH + "cat4.txt";
    const string CAT5 = COMPUTER_TRAINING_PATH + "cat5.txt";
    const string CAT6 = COMPUTER_TRAINING_PATH + "cat6.txt";
    const string CAT7 = COMPUTER_TRAINING_PATH + "cat7.txt";
    const string CAT8 = COMPUTER_TRAINING_PATH + "cat8.txt";
    const string CAT9 = COMPUTER_TRAINING_PATH + "cat9.txt";
    const string CAT10 = COMPUTER_TRAINING_PATH + "cat10.txt";

    const string DOG1 = COMPUTER_TRAINING_PATH + "dog1.txt";
    const string DOG2 = COMPUTER_TRAINING_PATH + "dog2.txt";
    const string DOG3 = COMPUTER_TRAINING_PATH + "dog3.txt";
    const string DOG4 = COMPUTER_TRAINING_PATH + "dog4.txt";
    const string DOG5 = COMPUTER_TRAINING_PATH + "dog5.txt";
    const string DOG6 = COMPUTER_TRAINING_PATH + "dog6.txt";
    const string DOG7 = COMPUTER_TRAINING_PATH + "dog7.txt";
    const string DOG8 = COMPUTER_TRAINING_PATH + "dog8.txt";
    const string DOG9 = COMPUTER_TRAINING_PATH + "dog9.txt";
    const string DOG10 = COMPUTER_TRAINING_PATH + "dog10.txt";

    const string PROFESSOR1 = COMPUTER_TRAINING_PATH + "professor1.txt";
    const string PROFESSOR2 = COMPUTER_TRAINING_PATH + "professor2.txt";
    const string PROFESSOR3 = COMPUTER_TRAINING_PATH + "professor3.txt";
    const string PROFESSOR4 = COMPUTER_TRAINING_PATH + "professor4.txt";
    const string PROFESSOR5 = COMPUTER_TRAINING_PATH + "professor5.txt";
    const string PROFESSOR6 = COMPUTER_TRAINING_PATH + "professor6.txt";
    const string PROFESSOR7 = COMPUTER_TRAINING_PATH + "professor7.txt";
    const string PROFESSOR8 = COMPUTER_TRAINING_PATH + "professor8.txt";
    const string PROFESSOR9 = COMPUTER_TRAINING_PATH + "professor9.txt";
    const string PROFESSOR10 = COMPUTER_TRAINING_PATH + "professor10.txt";



    // Parameters to modify
    int clusters = 10;

    vector<string> cat = {CAT1, CAT2, CAT3, CAT4, CAT5, CAT6, CAT7, CAT8, CAT9, CAT10};
    vector<string> dog = {DOG1, DOG2, DOG3, DOG4, DOG5, DOG6, DOG7, DOG8, DOG9, DOG10};
    vector<string> professor = {PROFESSOR1, PROFESSOR2, PROFESSOR3, PROFESSOR4, PROFESSOR5, PROFESSOR6, PROFESSOR7, PROFESSOR8, PROFESSOR9, PROFESSOR10};


    // Add words to train
    vector<string> words = {"cat", "dog", "professor"};
    vector<vector<string>> files = {cat, dog, professor};

    auto file = files.begin();
    for(auto word = words.begin(); word != words.end(); word++, file++) {
        train(*word, *file, clusters);
    }
    cout << "done" << endl;
}