import React, { Component } from 'react';
import OldConversation from './OldConversation';

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

    // TODO: Sort conversation array base on chronological order

    return conversations.map((timeStamp, id) => (
      <OldConversation key={id} timeStamp={timeStamp} changeConversation={this.props.changeConversation}/>
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