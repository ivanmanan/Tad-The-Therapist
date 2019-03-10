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
        //calculate random starting centroids
        for (int m = 0; m < 14; m++)
        {
            double next = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
            newCentroid[m] = next;
            //cout << newCentroid[m] << " ";
        }
        centroids.push_back(newCentroid);
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
            for (int r = 0; r < nextClustersData[f].size(); r++) {
                for (int c = 0; c < nextClustersData[f][r].size(); c++) {
                    cout << nextClustersData[f][r][c] << " ";
                }
                cout << endl;
            }
        }

        //set centroids to the averages of the new clusters
        for (int i = 0; i < numClusters; i++) {
            if (!nextClustersData[i].empty) 
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
    
    // Export training set as text file for that word
    string word_path = COMPUTER_WORDS_PATH + word + ".txt";
    

    vector<double> vtest = {1, 2, 3, 4, 5.5, -6};
    ofstream wordfile(word_path);

    // Need to adjust based on the number of states
    if(wordfile.is_open()) {

        for(auto it = vtest.begin(); it != vtest.end(); it++) {
            string valueToFile = to_string(*it) + " ";
            wordfile << valueToFile;
        }
        wordfile.close();
    }


}

int main() {

    // Training set for the word "cat"
    const string CAT1 = COMPUTER_TRAINING_PATH + "cat1.txt";
    const string CAT2 = COMPUTER_TRAINING_PATH + "cat2.txt";
    const string CAT3 = COMPUTER_TRAINING_PATH + "cat3.txt";
    const string CAT4 = COMPUTER_TRAINING_PATH + "cat4.txt";
    const string CAT5 = COMPUTER_TRAINING_PATH + "cat5.txt";

    // Parameters to modify
    int clusters = 5;

    vector<string> cat = {CAT1, CAT2, CAT3, CAT4, CAT5};

    // Add words to train
    vector<string> words = {"cat"};
    vector<vector<string>> files = {cat};

    auto file = files.begin();
    for(auto word = words.begin(); word != words.end(); word++, file++) {
        train(*word, *file, clusters);
    }
    cout << "done" << endl;
}