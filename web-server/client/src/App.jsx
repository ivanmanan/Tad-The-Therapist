import React, { Component } from 'react';

import Clock from './Components/Clock';
import Sidebar from './Components/Sidebar';
import Conversation from './Components/Conversation';

/*
  NOTE: Sidebar and Conversation are sibling components that rely on information between each other

  What if user clicks on past conversation in the middle of an active convseration?
  How do we know when the converation is over?
*/

class App extends Component {

  constructor(props) {
    super(props);
    this.state = {
      conversation: "none", // "active" is triggered by a new .txt file; "past" is mouse click
      timeStamp: "", // mouse click changes time stamp OR when new .txt file is made
      conversationHistory: [], // Array holding all dates/times of conversation histories
      currentDialogue: [] // Tad always speaks first; do a MySQL query based on current timeStamp
    };
  }

  // Using mock data
  componentDidMount() {
    this.setState({
      conversation: "past",
      timeStamp: "Dec 30, 2018, 9:55:22 PM",
      conversationHistory: ["Dec 30, 2018, 9:55:22 PM", "Fake Date"],
      currentDialogue: ["Hello. My name is Tad. How can I help you?", "I am sad.", "Why are you sad?", "My girlfriend broke-up with me.", "You should probably move on with your life. It is probably for the best. There is nothing you can do now to fix anything. You screwed up."]
    });
  }

  // Make functions that triggers mouse clicks and MySQL queries

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
            <Sidebar conversationHistory={this.state.conversationHistory}/>
          </div>
          <div className="text-center col-md-1">
            <div className="vl-large"></div>
          </div>
          <div className="text-center col-md-8">
            <Conversation currentDialogue={this.state.currentDialogue}/>
          </div>
        </div>

      </div>
    );
  }
}

export default App;
