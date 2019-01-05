import React, { Component } from 'react';
import io from 'socket.io-client'

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
    
    // Socket client that listens to file changes from the LCDK
    this.socket = io('http://localhost:3001');
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

    // Initialize socket connection
    this.socket.on('INITIALIZE', data => {
      console.log(data.message);
    });

    this.socket.on('DIALOGUE', data => {
      const tad = data.tad;
      const client = data.client;
      // Only initialize if state is not already active
      if (this.state.conversation !== "active") {
        // Generate new time stamp
        const DATE_OPTIONS = { year: 'numeric', month: 'short', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric' };
        const date = new Date();
        const new_timeStamp = date.toLocaleDateString('en-US', DATE_OPTIONS);
        this.setState({
          conversation: "active",
          timeStamp: new_timeStamp,
          currentDialogue: [tad]
        });
      }
      else {
        // Append to current dialogue latest messages
        let new_dialogue = this.state.currentDialogue;
        new_dialogue.push(client);
        new_dialogue.push(tad);
        this.setState({
          currentDialogue: new_dialogue
        });
      }
    });
  }

  componentWillUnmount() {
    this.socket.close();
  }

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

  // Save current conversation
  saveConversation() {
    // Retrieve time stamp
    const new_timeStamp = this.state.timeStamp;
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
      .then(() => {
        // Update conversation history to include newest time stamp addition
        let new_conversationHistory = this.state.conversationHistory;
        new_conversationHistory.unshift(new_timeStamp);
        this.setState({
          conversation: "past",
          time_stamp: new_timeStamp,
          conversationHistory: new_conversationHistory
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
