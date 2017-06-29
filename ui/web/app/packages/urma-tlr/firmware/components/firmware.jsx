import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import {FirmwareManager} from '../firmware-manager';
import FirmwareUpdateManual from './firmware-update-manual.jsx';

//TODO: auto-update firmware

export default class Firmware extends Component {
    constructor(props){
        super(props);
        this.state = {
            isFWUpdateReady: false
        };
    }

    componentWillMount() {
        this.firmwareManager = new FirmwareManager();
    }
    componentWillUnmount() {
        this.firmwareManager.removeAllListeners();
        this.firmwareManager = null;
    }

    onReady(isReady) {
        this.setState({isFWUpdateReady: isReady});
    }

    submitFirmwareUpdate(e) {
        this.firmwareManager.startUpdate(e);
    }
    render() {
        return (
            <div>
                <h3>Update Firmware</h3>
                <FirmwareUpdateManual firmwareManager={this.firmwareManager}
                                      onReady={this.onReady.bind(this)}/>
                <br/>
                <button type="button"
                        onClick={this.submitFirmwareUpdate.bind(this)}
                        disabled={!this.state.isFWUpdateReady}>Update Firmware
                </button>

            </div>
        );
    }
}
