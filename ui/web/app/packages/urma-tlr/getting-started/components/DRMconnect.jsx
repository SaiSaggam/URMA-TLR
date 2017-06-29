import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';

let alerts = Urma.alerts;

export default class DRMConnect extends Component {
    
    componentWillMount(){
        this.userName = "";
        this.password = "";
        this.props.buttonConfig(
            '/getting-started/firmware-update',
            '/getting-started/about-dashboard',
            this.addDeviceToRemoteManger.bind(this));
        this.props.disableNext();
    }

    getMeteorData() {
        let State = this.context.device.State;
        let Settings = this.context.device.Settings;
        let SubsManager = this.context.device.SubsManager;

        let cloudIntState = {};
        let cloudIntSettings = {};
        let cloudDeviceId ="";
        let cloudServer ="";
        let search = {_groupName: 'cloud',_groupIndex: 0};
        let stateSubscription = SubsManager.subscribe('state', search);
        let settingsSubscription = SubsManager.subscribe('settings', search);
        if (settingsSubscription.ready() && stateSubscription.ready()) {
            cloudIntState = State.findOne(search);
            cloudIntSettings = Settings.findOne(search);
            cloudServer = cloudIntSettings['server'];
            cloudDeviceId = cloudIntState['deviceid'];
        }
        return {
            cloudServer: cloudServer,
            cloudDeviceId: cloudDeviceId,
            cloudIntSettings: cloudIntSettings
        };
    }

    userNameHandler(event){
        this.userName = event.target.value;
    }

    passwordHandler(event){
        this.password = event.target.value;
    }

    addDeviceToRemoteManger(e) {
      let cloudServer = (this.data.cloudServer == '' ? 'my.devicecloud.com' : this.data.cloudServer);
      Meteor.call('provisionDevice',
            this.userName,
            this.password,
            this.data.cloudDeviceId,
            cloudServer,
            this.RMCallBackHandler.bind(this));
        return {
            transitionToNext: false
        };
    }

    RMCallBackHandler(error,result){
        if(error){
            this.showError(error.error, error.reason);
        }else {
            if(result.error){
                this.showError(result.error.error, result.error.reason);
            }else{
                /** Update cloud state to on if off**/
                let Settings = this.context.device.Settings;
                let cloudIntSettings = this.data.cloudIntSettings;
                let server = (cloudIntSettings.server == '' ? 'my.devicecloud.com' : cloudIntSettings.server);
                if (cloudIntSettings.state != 'on' || cloudIntSettings.server == '' ) {
                    Settings.update(cloudIntSettings._id, {$set: {'state': 'on','server':server}});
                }
                alerts.success('This device has been added to your Remote Manager account.', 'Provision Success');
                this.props.transitionToNext();
            }
        }
    }

    showError(error, reason) {
        var errCode = error || '500';
        var reason = reason || 'Unable to read the device list.';
        alerts.error(errCode+' - ' + reason, 'Provision Failed');
    }

    onSkip(){
        this.props.transitionToNext();
    }

    render(){
        return (
            <div className="welcome-getting-started">
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR_frontPanel.jpg" alt="Front Panel"/>
                    </div>
                    <div className="wizard-content-text">
                        <p>Digi Remote Manager provides centralized and automated management of your Digi devices.</p>
                        <p>Enter the username and Password for an existing Digi Remote Manager account.</p>
                        <a href="http://www.digi.com/products/cloud/digi-remote-manager" target="_blank">http://www.digi.com/products/cloud/digi-remote-manager</a>
                    </div>

                </div>
                <div className="wizard-content-area-right">
                    {(Object.keys(this.data.cloudIntSettings)).length === 0 ?
                        <p>Loading...</p>
                        :
                        <ProvisionForm enableNext={this.props.enableNext.bind(this)}
                                       userNameHandler={this.userNameHandler.bind(this)}
                                       passwordHandler={this.passwordHandler.bind(this)}/>
                    }
                    <div>
                        <a href="javascript:void(0)" className='float-right' onClick={this.onSkip.bind(this)}>Skip to Dashboard >></a>
                    </div>
                </div>
            </div>
        )
    }
};

reactMixin(DRMConnect.prototype, ReactMeteorData);

DRMConnect.contextTypes = {
    device: PropTypes.object.isRequired
};

class ProvisionForm extends Component {
    componentWillMount(){
        this.props.enableNext();
    }

    render(){
        return (
            <form>
                <div>
                    <div className="box">
                        <label>Username</label>
                        <input type="text" className="form-control" onChange={this.props.userNameHandler.bind(this)} placeholder="username"/>
                    </div>
                    <div className="box">
                        <label>Password</label>
                        <input type="password" className="form-control" onChange={this.props.passwordHandler.bind(this)} placeholder="password"/>
                    </div>
                    <p>Click NEXT to login.</p>
                </div>
            </form>
        );
    }
}
ProvisionForm.propTypes = {
    enableNext: PropTypes.func.isRequired,
    userNameHandler: PropTypes.func.isRequired,
    passwordHandler: PropTypes.func.isRequired
};