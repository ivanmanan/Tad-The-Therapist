import React, { Component } from 'react';
import { Button } from 'react-bootstrap';

class Conversation extends Component {
  constructor(props) {
    super(props);
    this.displayConvesation = this.displayConversation.bind(this);
    this.getButtonView = this.getButtonView.bind(this);
    this.saveConversation = this.saveConversation.bind(this);
  }

  // Tad always starts the dialogue
  // Tad always ends the dialogue
  // Always assume the conversation is alternating --- no double texting
  displayConversation() {
    const currentDialogue = this.props.currentDialogue;

    // if id % 2 == 0 --> This is Tad speaking; otherwise it is client speaking
    return currentDialogue.map((dialogue, id) => (
      <div className="dialogue" key={id}>
        {id % 2 === 0 ? (
          <div className="Tad-speaking text-left">
            <p>{dialogue}</p>
          </div>
        ) : (
          <div className="Client-speaking text-right">
            <p>{dialogue}</p>
          </div>
          )}
      </div>
    ));
  }


  saveConversation() {
    const currentDialogue = this.props.currentDialogue;

    // Retrieve time stamp
    const DATE_OPTIONS = { year: 'numeric', month: 'short', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric' };
    let date = new Date();
    let timeStamp = date.toLocaleDateString('en-US', DATE_OPTIONS);

    fetch('/conversation', {
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      method: 'POST',
      body: JSON.stringify({
        timeStamp: timeStamp,
        dialogue: currentDialogue
      })
    });

    // TODO: Create function that calls App.jsx to retrieve entirety of hash table again and change its array of conversations time stamps state
    /*
      .then(res => res.json())
      .then(() => {
        console.log("SAVED!");
      })
      */


  // Save button submits post request with dialogue array passed into node.js --> node.js makes the text file
  // After pressing the save button, the parent states should change to be past conversation
  // If it is past conversation, then be able to delete conversation

  }

  getButtonView() {
    switch(this.props.conversation) {
      case "active":
      return(<Button id="save-button" onClick={this.saveConversation}><p>Save Conversation</p></Button>);
      case "past":
        return(<Button id="delete-button" onClick={this.props.deleteConversation}><p>Delete Conversation</p></Button>);
      case "none":
      default:
        return(<p>Start a conversation!</p>);
    }
  }

  render() {
    return (
      <div className="Conversation">
        {this.displayConversation()}

        <div className="Save-Conversation text-center">
          {this.getButtonView()}
        </div>
      </div>
    );
  }
}

export default Conversation;