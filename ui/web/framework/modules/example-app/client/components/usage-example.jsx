import React from 'react';

export default class UsageExample extends React.Component {

    render() {
        return (
            <div>
                <p>Usage Example</p>
                <p>Path to this page: {this.props.location ? this.props.location.pathname : 'unknown'}</p>
            </div>
        );
    }
}
