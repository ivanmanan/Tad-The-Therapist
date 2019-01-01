import React, { Component } from 'react';

import Clock from './Components/Clock';
import Sidebar from './Components/Sidebar';
import Conversation from './Components/Conversation';

/*
  In active conversation, I must be able to append to the currentDialogue array in App.jsx
  when new .txt file gets added

  Perhaps have .txt files, where each line is a dialogue to be pushed into the array
  File paths stored in MySQL database
*/


class App extends Component {

  constructor(props) {
    super(props);
    this.state = {
      conversation: "active", // "active" is triggered by a new .txt file; "past" is mouse click; "none" is default
      timeStamp: "", // mouse click changes time stamp OR when new .txt file is made
      conversationHistory: [], // Array holding all dates/times of conversation histories
      currentDialogue: [] // Tad always speaks first; do a MySQL query based on current timeStamp
    };
    this.changeConversation = this.changeConversation.bind(this);
  }

  componentDidMount() {

    // Do initial SQL query on conversationHistory
    fetch('/conversations', {
      headers: {
        'Content-Type': 'application/json'
      },
      method: 'GET'
    })
      .then(res => res.json())
      .then(history => {this.setState({conversationHistory: history})});


    // This is mock data for testing purposes
    /*
    this.setState({
      conversation: "past",
      timeStamp: "Dec 30, 2018, 9:55:22 PM",
      conversationHistory: ["Dec 30, 2018, 9:55:22 PM", "Fake Date"],
      currentDialogue: ["Hello. My name is Tad. How can I help you?", "I am sad.", "Why are you sad?", "My girlfriend broke-up with me.", "You should probably move on with your life. It is probably for the best. There is nothing you can do now to fix anything. You screwed up."]
    });
    */
  }

  // Make function when new conversation is initialized via new text file is recognized
  // When conversation is over, I submit a POST request
  // Can have a save button that ends conversation

  // When old conversation was selected
  changeConversation(new_timeStamp) {

    // This is a GET request which accesses the time stamp value from hash table in Node.js



    console.log(new_timeStamp);
    this.setState({
      conversation: "past",
      timeStamp: new_timeStamp,
      currentDialogue: ["Hello, I am Tad.", "Why?", "Why not?"]
    });
  }

  render() {
    return (
      <div className="App container">

        <div className="Titlebar row">
          <div className="text-center col-md-3">
              <h1>History</h1>
          </div>
          <div className="text-center col-md-1">
            <div className="vl-small"></div>
          </div>
          <div className="text-center col-md-8">
            <Clock conversation={this.state.conversation} timeStamp={this.state.timeStamp}/>
          </div>
        </div>
        
        <hr id="title-hr"></hr>

        <div className="Content row">
          <div className="text-center col-md-3">
            <Sidebar conversationHistory={this.state.conversationHistory} changeConversation={this.changeConversation}/>
          </div>
          <div className="text-center col-md-1">
            <div className="vl-large"></div>
          </div>
          <div className="text-center col-md-8">
            <Conversation conversation={this.state.conversation} currentDialogue={this.state.currentDialogue}/>
          </div>
        </div>

      </div>
    );
  }
}

export default App;
