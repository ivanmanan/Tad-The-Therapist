import React, { Component } from 'react';
import { Button } from 'react-bootstrap';

class Conversation extends Component {
  constructor(props) {
    super(props);
    this.displayConvesation = this.displayConversation.bind(this);
    this.getButtonView = this.getButtonView.bind(this);
  }

  // Tad always starts and ends the dialogue
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

  getButtonView() {
    switch(this.props.conversation) {
      case "active":
      return(<Button id="save-button" onClick={this.props.saveConversation}><p>Save Conversation</p></Button>);
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