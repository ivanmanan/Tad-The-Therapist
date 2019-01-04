import React, { Component } from 'react';

import Clock from './Components/Clock';
import Sidebar from './Components/Sidebar';
import Conversation from './Components/Conversation';

class App extends Component {

  constructor(props) {
    super(props);
    this.state = {
      conversation: "none", // "active" is triggered by a new .txt file; "past" is mouse click; "none" is default
      timeStamp: "", // mouse click changes time stamp OR when new .txt file is made
      conversationHistory: [], // Array holding all dates/times of conversation histories
      currentDialogue: [] // Tad always speaks first; do a MySQL query based on current timeStamp
    };
    this.changeConversation = this.changeConversation.bind(this);
    this.saveConversation = this.saveConversation.bind(this);
    this.deleteConversation = this.deleteConversation.bind(this);
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
      .then(history => {
        let time_stamps = [];
        // Convert JSON object to an array
        for (let i = history.length-1; i >= 0; i--) {
          time_stamps.push(history[i].Time_Stamp);
        }
        this.setState({conversationHistory: time_stamps});
      });
  }

  // Make function when new conversation is initialized via new text file is recognized
  // When conversation is over, I submit a POST request
  // Can have a save button that ends conversation

  /*
    In active conversation, I must be able to append to the currentDialogue array in App.jsx
    when new dialogue gets added into the .txt file
  */

  // When old conversation was selected
  changeConversation(new_timeStamp) {
    // GET request which access the conversation given time stamp key
    fetch('/conversation/' + new_timeStamp, {
      headers: {
        'Content-Type': 'application/json'
      },
      method: 'GET'
    })
      .then(res => res.json())
      .then(dialogue => {
        this.setState({
          conversation: "past",
          timeStamp: new_timeStamp,
          currentDialogue: dialogue
        });
      });
  }

  // TODO: Test this function using Conversation.jsx and Server.js
  // Save current conversation
  saveConversation() {
    // Retrieve time stamp
    const DATE_OPTIONS = { year: 'numeric', month: 'short', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric' };
    const date = new Date();
    const new_timeStamp = date.toLocaleDateString('en-US', DATE_OPTIONS);
    // Submit POST request
    fetch('/conversation', {
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      method: 'POST',
      body: JSON.stringify({
        time_stamp: new_timeStamp
      })
    })
      .then(res => res.json())
      .then(dialogue => {
        // Update conversation history with newest time stamp addition
        // TODO: test to make sure latest conversation gets pushed to the top of the History sidebar
        let new_conversationHistory = this.state.conversationHistory;
        new_conversationHistory.push(new_timeStamp);
        this.setState({
          conversation: "past",
          time_stamp: new_timeStamp,
          conversationHistory: new_conversationHistory,
          currentDialogue: dialogue
        });
      });
}

  // Delete current conversation
  deleteConversation() {
    // Submit DELETE request
    fetch('/conversation', {
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      method: 'DELETE',
      body: JSON.stringify({
        time_stamp: this.state.timeStamp
      })
    })
      .then(() => {
        // Update state by removing element in conversation history
        const new_conversationHistory = this.state.conversationHistory.filter(convo => convo !== this.state.timeStamp);
        this.setState({
          conversation: "none",
          timeStamp: "",
          conversationHistory: new_conversationHistory, // Conversation history gets updated
          currentDialogue: []
        });
      })
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
            <Conversation conversation={this.state.conversation} currentDialogue={this.state.currentDialogue} saveConversation={this.saveConversation} deleteConversation={this.deleteConversation}/>
          </div>
        </div>

      </div>
    );
  }
}

export default App;
