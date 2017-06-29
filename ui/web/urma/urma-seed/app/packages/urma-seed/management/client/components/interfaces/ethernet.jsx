import { Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import StateDetails from './ethernet-state.jsx';

let Link = Urma.ContextualLink;

export default class Ethernet extends Component {

    getMeteorData() {
        let Settings = this.context.Settings;
        let State = this.context.State;
        let SubsManager = this.context.SubsManager;

        let eth = {};
        let ethState = {};
        let settingsSearch = {_groupName: 'ethernet', _groupIndex: parseInt(this.props.params._groupIndex)};
        let stateSearch = {_groupName: 'eth', _groupIndex: parseInt(this.props.params._groupIndex)};

        let settingsSubscription = SubsManager.subscribe('settings', settingsSearch);
        let stateSubscription = SubsManager.subscribe('state', stateSearch);
        if (settingsSubscription.ready() && stateSubscription.ready()) {
            eth = Settings.findOne(settingsSearch);
            ethState = State.findOne(stateSearch);
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
        let Settings = this.context.Settings;

        let self = this;
        let eth = this.data.eth;
        let ethState = this.data.ethState;
        let toggleValueFn = () => self.state.viewDetailsOpen ? 'down' : 'right';

        return (
            <div>
                <h1>{eth.name}</h1>
                <h3>Current Status: <span className='neutral'>{ethState.admin_status}</span></h3>
                <button className="toggle-link" onClick={this.handleClick.bind(this)}>View Details <i className={'fa fa-caret-' + toggleValueFn()}></i></button>
                <StateDetails  openState={this.state.viewDetailsOpen} ethState={ethState}/>
                <h3>Configure</h3>
                <form>
                    <div className='form-group radio'>
                        <label>
                            <input type='radio' name='DHCP' checked={eth.dhcpcli === 'on'}
                                   onChange={(e)=>Settings.update(eth._id, {$set: {dhcpcli: 'on'}})}/>
                            Automatically configure IP settings using DHCP
                        </label>
                    </div>
                    <div className='form-group radio'>
                        <label>
                            <input type='radio' name='DHCP' checked={eth.dhcpcli !== 'on'}
                                   onChange={(e)=>Settings.update(eth._id, {$set: {dhcpcli: 'off'}})}/>
                            Configure IP settings manually
                        </label>
                    </div>
                    <div className='form-group'>
                        <label>IP Address</label>
                        <input type='text' className='form-control' defaultValue={eth.IPaddr}
                               onChange={(e)=>Settings.update(eth._id, {$set: {IPaddr: e.target.value}})}
                               disabled={eth.dhcpcli === 'on'}/>
                    </div>
                    <div className='form-group'>
                        <label>Subnet Mask</label>
                        <input type='text' className='form-control' defaultValue={eth.mask}
                               onChange={(e)=>Settings.update(eth._id, {$set: {mask: e.target.value}})}
                               disabled={eth.dhcpcli === 'on'}/>
                    </div>
                    <div className='form-group'>
                        <label>Gateway</label>
                        <input type='text' className='form-control' defaultValue={eth.gateway}
                               onChange={(e)=>Settings.update(eth._id, {$set: {gateway: e.target.value}})}
                               disabled={eth.dhcpcli === 'on'}/>
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
    SubsManager: React.PropTypes.object.isRequired,
    Settings: React.PropTypes.object.isRequired,
    State: React.PropTypes.object.isRequired
};
