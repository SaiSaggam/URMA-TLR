import {default as React, Component, PropTypes } from 'react';

export default class ProgressBar extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        let percent = this.props.percent;
        if (percent < 0) {percent = 0};
        if (percent > 100) {percent = 100};
        let barStyle = {
            width: percent+'%'
        };

        return (
            <div>
                <div>{this.props.text}</div>
                <div className='progress-bar-wrapper'>
                    <div className='progress-bar' style={barStyle}></div>
                </div>
                <div>{percent}%</div>
            </div>
        );
    }
}

ProgressBar.propTypes = {
    percent: React.PropTypes.number,
    text: React.PropTypes.string
};
ProgressBar.defaultProps = {
    percent: 0,
    text: ''

};

