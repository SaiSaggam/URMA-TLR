import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import StateDetails from './ethernet-state.jsx';
import Urma from 'meteor/digi:urma-core';

let Link = Urma.ContextualLink;

export default class Ethernet extends Component {

    getMeteorData() {
        let Settings = this.context.device.Settings;
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let eth = {};
        let ethState = {};
        let search =    {_groupName: 'eth', _groupIndex: parseInt(this.props.params._groupIndex)};

        let settingsSubscription = SubsManager.subscribe('settings', search);
        let stateSubscription = SubsManager.subscribe('state', search);
        if (settingsSubscription.ready() && stateSubscription.ready()) {
            eth = Settings.findOne(search);
            ethState = State.findOne(search);
        }

        return {
            eth: eth,
            ethState: ethState
        };
    }

    constructor(props){
        super(props);
        this.state = { viewDetailsOpen: false};
    }

    handleClick(e) {
        this.setState({viewDetailsOpen: !this.state.viewDetailsOpen});
    }

    renderEth() {
        let Settings = this.context.device.Settings;

        let self = this;
        let eth = this.data.eth;
        let ethState = this.data.ethState;
        let toggleValueFn = () => self.state.viewDetailsOpen ? 'down' : 'right';

        return (
            <div>
                <h1>{eth.name}</h1>
                <h3>Current Status: <span className='neutral'>{ethState['admin_status']}</span></h3>
                <button className="toggle-link" onClick={this.handleClick.bind(this)}>View Details <i className={'fa fa-caret-' + toggleValueFn()}></i></button>
                <StateDetails  openState={this.state.viewDetailsOpen} ethState={ethState}/>
                <h3>Configure</h3>
                <form>
                    <div className='form-group'>
                        <label>Description</label>
                        <input type='text' className='form-control' defaultValue={eth['description']}
                               onChange={(e)=>Settings.update(eth._id, {$set: {'description': e.target.value}})}/>
                    </div>
                    <div className='form-group radio'>
                        <label>
                            <input type='radio' name='DHCP' checked={eth['dhcp'] === 'on'}
                                   onChange={(e)=>Settings.update(eth._id, {$set: {'dhcp': 'on'}})}/>
                            Automatically configure IP settings using DHCP
                        </label>
                    </div>
                    <div className='form-group radio'>
                        <label>
                            <input type='radio' name='DHCP' checked={eth['dhcp'] !== 'on'}
                                   onChange={(e)=>Settings.update(eth._id, {$set: {'dhcp': 'off'}})}/>
                            Configure IP settings manually
                        </label>
                    </div>
                    <div className='form-group'>
                        <label>IP Address</label>
                        <input type='text' className='form-control' defaultValue={eth['ip_address']}
                               onChange={(e)=>Settings.update(eth._id, {$set: {'ip_address': e.target.value}})}
                               disabled={eth['dhcp'] === 'on'}/>
                    </div>
                    <div className='form-group'>
                        <label>Subnet Mask</label>
                        <input type='text' className='form-control' defaultValue={eth['mask']}
                               onChange={(e)=>Settings.update(eth._id, {$set: {'mask': e.target.value}})}
                               disabled={eth['dhcp'] === 'on'}/>
                    </div>
                </form>
            </div>
        )
    }

    render() {
        return (
            <div style={{padding: '10px'}}>
                {this.data.eth._id ? this.renderEth() : <p>Loading...</p>}
            </div>
        );
    }
}

reactMixin(Ethernet.prototype, ReactMeteorData);

Ethernet.contextTypes = {
    device: PropTypes.object.isRequired
};
