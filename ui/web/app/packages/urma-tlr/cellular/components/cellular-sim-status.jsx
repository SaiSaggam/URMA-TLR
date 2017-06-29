import {default as React, Component, PropTypes } from 'react';

export default class CellularSimStatus extends Component {
    render() {
        return (
            <div style={{paddingTop: '30px'}}>
                <div className="simStatus">
                    <p><label>SIM Status:</label><span className="bold">{this.props.cellularIntState['sim_status']}</span></p>
                    <p><label>ICCID:</label><span className="bold">{this.props.cellularIntState['iccid']}</span></p>
                    <p><label>Signal Strength:</label><span className="bold">{this.props.cellularIntState['signal_strength']}</span></p>
                    <p><label>Signal Quality:</label><span className="bold">{this.props.cellularIntState['signal_quality']}</span></p>
                </div>
            </div>
        );
    }
}

CellularSimStatus.propTypes = {
    cellularIntState: PropTypes.object.isRequired
};