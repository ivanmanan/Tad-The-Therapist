import React, { Component } from 'react';

/*
  Need to trigger when new .txt file exists and then initialize new conversation
  Must save date string for parent class
*/

const DATE_OPTIONS = { year: 'numeric', month: 'short', day: 'numeric', hour: 'numeric', minute: 'numeric', second: 'numeric' };
function FormattedDate(props) {
  return (
    <h2>{props.date.toLocaleDateString('en-US', DATE_OPTIONS)}</h2>
  );
}

class Clock extends Component {
    
    constructor(props) {
        super(props);
        this.state = {date: new Date()};
        this.getClock = this.getClock.bind(this);
    }

    componentDidMount() {
      this.timerID = setInterval(
        () => this.tick(),
        1000
      );
    }

    tick() {
      this.setState({
        date: new Date()
      });
    }

    getClock() {
      switch(this.props.conversation) {
        case "active":
          return(<h2>Active Conversation at {this.props.timeStamp}</h2>);
        case "past":
          return(<h2>Past Conversation at {this.props.timeStamp}</h2>);
        case "none":
        default:
          return(<FormattedDate date={this.state.date}/>);
      }
    }

    render() {
        return (
            <div className="Clock">
              {this.getClock()}
            </div>
        );
    }
}

export default Clock;