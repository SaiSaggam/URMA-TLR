import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import CellularSimStatus from '../../cellular/components/cellular-sim-status.jsx';

export default class CellularResults extends Component {

    getMeteorData() {
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;
        let cellularIntState = null;
        let search = {_groupName: 'cellular'};
        let stateSubscription = SubsManager.subscribe('state', search);
        if (stateSubscription.ready()) {
            cellularIntState = State.findOne(search);
        }
        return {
            cellularIntState: cellularIntState
        };
    }

    componentWillMount(){
        this.props.buttonConfig('/getting-started/cellular/0', '/getting-started/firmware-update');
        this.props.enableNext();
    }

    renderCellularResults(){
        return (
            <div>
                <h6>Cellular Connection Results</h6>
                <div className="cellularResults">
                    <p><label>Description: </label><span className="bold">{this.data.cellularIntState['description']}</span></p>
                    <p><label>Module: </label><span className="bold">{this.data.cellularIntState['module']}</span></p>
                    <p><label>Firmware Version: </label><span className="bold">{this.data.cellularIntState['firmware_version']}</span></p>
                    <p><label>Hardware Version: </label><span className="bold">{this.data.cellularIntState['hardware_version']}</span></p>
                    <p><label>IMEI: </label><span className="bold">{this.data.cellularIntState['imei']}</span></p>
                    <p><label>Registration Status: </label><span className="bold">{this.data.cellularIntState['registration_status']}</span></p>
                    <p><label>Network Provider: </label><span className="bold">{this.data.cellularIntState['network_provider']}</span></p>
                    <p><label>Temperature: </label><span className="bold">{this.data.cellularIntState['temperature']}</span></p>
                    <p><label>Connection Type: </label><span className="bold">{this.data.cellularIntState['connection_type']}</span></p>
                    <p><label>Radio Band: </label><span className="bold">{this.data.cellularIntState['radio_band']}</span></p>
                    <p><label>Channel: </label><span className="bold">{this.data.cellularIntState['channel']}</span></p>
                    <p><label>PDP Context: </label><span className="bold">{this.data.cellularIntState['pdp_context']}</span></p>
                    <p><label>IP Address: </label><span className="bold">{this.data.cellularIntState['ip_address']}</span></p>
                    <p><label>Mask: </label><span className="bold">{this.data.cellularIntState['mask']}</span></p>
                    <p><label>Gateway: </label><span className="bold">{this.data.cellularIntState['gateway']}</span></p>
                    <p><label>DNS Servers: </label><span className="bold">{this.data.cellularIntState['dns_servers']}</span></p>
                    <p><label>RX Packets: </label><span className="bold">{this.data.cellularIntState['rx_packets']}</span></p>
                    <p><label>TX Packets: </label><span className="bold">{this.data.cellularIntState['tx_packets']}</span></p>
                    <p><label>RX Bytes: </label><span className="bold">{this.data.cellularIntState['rx_bytes']}</span></p>
                    <p><label>TX Bytes: </label><span className="bold">{this.data.cellularIntState['tx_bytes']}</span></p>
                </div>
            </div>
        )
    }

    render() {
        return (
            <div>
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR54_frontPanel.png" alt="Front Panel"/>
                        {this.data.cellularIntState?
                            <CellularSimStatus cellularIntState={this.data.cellularIntState}/>
                            :
                            <p>Loading...</p>
                        }
                    </div>
                    <div className="wizard-content-text">
                        <p>The device has connected to the cellular WAN.</p>
                        <p>Note the output to the right for details on cellular WAN connection.</p>
                        <p>Click on BACK to change any cellular connection parameters and to retry the WAN connection.</p>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    {this.data.cellularIntState?
                        this.renderCellularResults()
                        :
                        <p>Loading...</p>
                    }
                </div>
            </div>
        );
    }

}

reactMixin(CellularResults.prototype, ReactMeteorData);

CellularResults.contextTypes = {
    device: PropTypes.object.isRequired
};