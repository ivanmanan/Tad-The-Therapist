import React, { Component } from 'react';


/*
  In active conversation, I must be able to append to the array in parent component 
  when new .txt file gets added
*/

class Conversation extends Component {
  constructor(props) {
    super(props);
    this.displayConvesation = this.displayConversation.bind(this);
  }

  // Tad always starts the dialogue
  // Tad always ends the dialogue

  // Merge the two arrays into one
  // Always assume the conversation is alternating --- no double texting
  displayConversation() {
    const currentDialogue = this.props.currentDialogue;

    // if id % 2 == 0 --> This is Tad speaking; otherwise it is client speaking
    return currentDialogue.map((dialogue, id) => (
      <div className="dialogue" key={id}>
        {id % 2 == 0 ? (
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

  render() {
    return (
      <div className="Conversation">
        {this.displayConversation()}
      </div>
    );
  }
}

export default Conversation;