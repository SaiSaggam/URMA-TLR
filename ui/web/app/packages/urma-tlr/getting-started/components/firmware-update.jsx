import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import {FirmwareManager} from '../../firmware/firmware-manager';
import FirmwareUpdateManual from '../../firmware/components/firmware-update-manual.jsx';
import { withRouter } from 'react-router';

//TODO: remove the Update Firmware button and submit update on click of Next button.
//TODO: auto-update firmware

export class FWUpdate extends Component {
    constructor(props){
        super(props);
    }

    componentWillMount(){
        this.firmwareManager = new FirmwareManager();
        this.props.buttonConfig(
            this.props.router.goBack.bind(this),
            '/getting-started/remote-manager-connect',
            this.startFirmwareUpdate.bind(this));
        this.props.disableNext();
    }

    componentWillUnmount() {
        this.firmwareManager.removeAllListeners();
        this.firmwareManager = null;
    }

    onReady(isReady) {
        if (isReady) {
            this.props.enableNext();
        } else {
            this.props.disableNext();
        }
    }

    startFirmwareUpdate(e) {
        this.firmwareManager.startUpdate(e);
        return {
            transitionToNext: false
        };
    }

    onFirmwareUpdateComplete(successful) {
        if (successful) {
            this.props.transitionToNext();
        }
    }


    onSkip(){
        this.props.transitionToNext();
    }

    render() {
        return (
            <div>
                <h1>Firmware Update</h1>
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR_frontPanel.jpg" alt="Front Panel"/>
                    </div>
                    <div className="wizard-content-text">
                        <p>Using the device's WAN connection, download and install the latest firmware version.</p>
                        <p>You many also install a firmware version from a locally saved file.</p>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    <FirmwareUpdateManual firmwareManager={this.firmwareManager}
                                          onReady={this.onReady.bind(this)}
                                          onComplete={this.onFirmwareUpdateComplete.bind(this)}/>
                    <div>
                        <br/>
                        <span className='float-right'>Click NEXT to update</span>
                        <br/>
                        <a href="javascript:void(0)" onClick={this.onSkip.bind(this)} className='float-right'>Skip to Remote Manager >></a>
                    </div>
                </div>
            </div>
        );
    }
}

export default withRouter(FWUpdate)
