import React, { Component } from 'react';
import { Button } from 'react-bootstrap';

class OldConversation extends Component {
  constructor(props) {
    super(props);
    this.handleClick = this.handleClick.bind(this);
  }

  handleClick() {
    this.props.changeConversation(this.props.timeStamp);
  }

  render() {
    return (
      <Button className="conversation-button" value={this.props.timeStamp} onClick={this.handleClick}>
        <p>{this.props.timeStamp}</p>
      </Button>    
    );
  }
}

export default OldConversation;