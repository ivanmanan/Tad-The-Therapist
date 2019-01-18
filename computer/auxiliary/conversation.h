#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <vector>
#include <string>
#include "duplex.h" // need to be used to read files

using namespace std;

bool conversate(const string INPUT_FILE_NAME) {

    // Read the file
    vector<string> inputs = readFile(INPUT_FILE_NAME);

    // Feed into neural network (or HMM)
    // TODO: This function gets written in neural_network.h
    //string client_message = neural_network(inputs);

    // Tad picks appropriate response to client message
    // TODO: This function gets written in chatbot.h
    //string Tad_message = TadResponse(client_message);

    // Write message into output.txt and Tad's response at once
    // TODO: This function gets written in duplex.h
    //writeOutput(client_message, Tad_message);

    // Determine if Tad's message explicitly says good-bye; if so, then end conversation
    // TODO: This function gets written in chatbot.h
    //if(finishConversation(Tad_message)) {
        // return true;    
    //}

    return false;
}

#endif