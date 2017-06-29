import {default as React, Component, PropTypes } from 'react';
import Urma from 'meteor/digi:urma-core';

let CountdownTimer = Urma.CountdownTimer;
let timeOut = null;
let reqInterval = null;

const CONNECTION_TIMEOUT = 120000;

export default class CelluarRetryDialog extends Component {

    constructor(props){
        super(props);
        this.state = {
            timedOut: false
        };
    }

    componentWillMount(){
        this.resetTimers();
    }

    componentWillUnmount(){
        this.clearTimers();
    }

    onTimeout() {
        this.setState({timedOut: true});
        this.clearTimers();
    }

    onRetry(e) {
        let Settings = this.props.deviceSettings;
        Settings.update(this.props.cellularSettingsId, {$set: {'apn': this.props.cellularAPN,'state': 'on'}});
        this.setState({timedOut: false});
        this.resetTimers();
    }

    resetTimers(){
        this.clearTimers();
        // test if we have ip address  on a regular interval
        reqInterval = setInterval(this.isConnected.bind(this), 10000);
        // show time out error in case we are unable to establish a connection in 2 minutes.
        timeOut = setTimeout(this.onTimeout.bind(this), CONNECTION_TIMEOUT);
    }

    clearTimers() {
        if (timeOut != null) {
            clearTimeout(timeOut);
            timeOut = null;
        }
        if (reqInterval != null) {
            clearInterval(reqInterval);
            reqInterval = null;
        }
    }

    isConnected() {
        if(this.props.cellularIntState['ip_address'] !="" && this.props.cellularIntState['ip_address'] !="0.0.0.0"){
            this.props.onSuccess(true);
        }
    }

    render() {
        return (
            <div style={{minWidth: '300px', maxWidth: '500px'}}>
                {!this.state.timedOut ?
                <div className="cellular-msg">
                    <div>
                        <h5>Please wait while your device connects.</h5>
                        <CountdownTimer timeRemaining={CONNECTION_TIMEOUT}/>
                    </div>
                    <div className="loader">Loading...</div>
                </div>
                   : 
                <div className="cellular-msg">
                    <h5>Alert</h5>
                    <p>Cannot connect to cellular network.</p>
                    <p className="error">Timed out while waiting for device to connect.</p>
                    <p>Retry ?</p>
                </div>
                }
                <div style={{float: 'right'}}>
                    {this.state.timedOut ?
                        <button type='button' id="retryBtn"  className="btn-gray"  style={{margin: '10px'}}
                                onClick={this.onRetry.bind(this)}>
                            Retry
                        </button>:null
                    }
                    <button type='button' id="cancelBtn" className="btn-gray" style={{margin: '10px'}}
                            onClick={this.props.onClose.bind(this)}>
                        Cancel
                    </button>
                </div>
            </div>
        );
    }
}

CelluarRetryDialog.propTypes = {
    onClose: PropTypes.func.isRequired,
    onSuccess: PropTypes.func.isRequired,
    cellularAPN: PropTypes.string.isRequired,
    deviceSettings: PropTypes.object.isRequired,
    cellularSettingsId: PropTypes.string.isRequired,
    cellularIntState: PropTypes.object.isRequired
};