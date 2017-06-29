import React from 'react';

let countdownInterval;

export default class CountdownTimer extends React.Component {
    constructor(props){
        super(props);
        this.state = {
            timeRemaining: 0
        };
        this.tick = this.tick.bind(this);
    }

    tick() {
        this.setState({timeRemaining: this.state.timeRemaining - 1000});
        if (this.state.timeRemaining <= 0) {
            clearInterval(this.interval);
        }
    }

    componentDidMount() {
        this.setState({ timeRemaining: this.props.timeRemaining });
        countdownInterval = setInterval(this.tick, 1000);
    }

    componentWillUnmount() {
        clearInterval(countdownInterval);
    }

    formatTime(milliseconds){
        let totalSeconds = Math.round(milliseconds / 1000);

        let seconds = parseInt(totalSeconds % 60);
        let minutes = parseInt(totalSeconds / 60) % 60;

        seconds = seconds < 10 ? '0' + seconds : seconds;
        minutes = minutes < 10 ? '0' + minutes : minutes;


        return minutes + ':' + seconds;
    }

    render() {
        return (
            <div className="countdown-timer">
                <p className="bold">Time Remaining</p>
                <p className="countdown-timer-remaining">{this.formatTime(this.state.timeRemaining)}</p>
            </div>
        );
    }
};

CountdownTimer.propTypes = {
    timeRemaining: React.PropTypes.number.isRequired
};