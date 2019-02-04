#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <vector>
#include <string>
#include "duplex.h"
#include "chatbot.h"

using namespace std;

bool conversate(const string INPUT_FILE_NAME, const string BUSY_FILE_NAME) {

    // Computer is in busy state
    // TODO: Implement busy.txt file with single character "0"
    //writeBusyFile(BUSY_FILE_NAME, "1");

    // Read the file
    vector<string> inputs = readFile(INPUT_FILE_NAME);

    // Feed into HMM
    // TODO: This function gets written in hmm.h
    //string client_message = hmm(inputs);

    // Tad picks appropriate response to client message
    // TODO: This function gets written in chatbot.h
    //string Tad_message = respond(client_message);

    // Write message into output.txt and Tad's response at once
    // TODO: This function gets written in duplex.h
    //writeOutput(client_message, Tad_message);

    // Determine if Tad's message explicitly says good-bye; if so, then end conversation
    // TODO: This function gets written in chatbot.h
    //if(finishConversation(Tad_message)) {
        // return true;    
    //}

    // Computer is not in busy state
    // TODO: Update busy.txt file with single character "1"
    //writeBusyFile(BUSY_FILE_NAME, "0");

    return false;
}

#endif