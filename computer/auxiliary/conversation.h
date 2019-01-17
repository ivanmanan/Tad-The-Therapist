#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <vector>
#include <string>
#include "duplex.h" // need to be used to read files

using namespace std;

bool conversate(const string FILE_NAME) {

    // Read the file
    vector<string> inputs = readFile(FILE_NAME);

    // Feed into neural network
    //string client_message = neural_network(inputs);

    // Tad picks appropriate response to client message
    // NOTE: May need to create a data structure to keep track of conversation history
    //       OR I can read from output.txt
    //string Tad_message = TadResponse(client_message);

    // Write message into output.txt and Tad's response at once
    //writeOutput(client_message, Tad_message);

    // Determine if Tad's message explicitly says good-bye; if so, then end conversation
    //if(finishConversation(Tad_message)) {
        // return true;    
    //}

    return false;
}

#endif