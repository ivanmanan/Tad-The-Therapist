import React, { Component } from 'react';
import { Button } from 'react-bootstrap';

class Sidebar extends Component {
  constructor(props) {
    super(props);
    this.displayConversationHistories = this.displayConversationHistories.bind(this);
  }


  displayConversationHistories() {
    const conversations = this.props.conversationHistory;

    if (conversations.length === 0) {
      return (<p>No past conversations.</p>)
    }

    // NOTE: Need to sort conversation array base on chronological order

    // Display each time-stamp as a block and can be triggered by mouse clicks
    // The mouse clicks in return must do a callback to the parent component
    // See Travel Share's omnibox.jsx on how to do this with buttons and callbacks
    return conversations.map((conversation, id) => (
      <div className="conversation-block" key={id}>
        <Button id="conversation-button">
          <p>{conversation}</p>
        </Button>
      </div>
    ));
  }


  render() {
    return (
      <div className="Sidebar">
        {this.displayConversationHistories()}
      </div>
    );
  }
}

export default Sidebar;