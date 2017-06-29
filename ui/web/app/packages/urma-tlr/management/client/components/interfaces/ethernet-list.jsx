import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';

let ContextualLink = Urma.ContextualLink;

export default class EthernetList extends Component {

    getMeteorData() {
        let Settings = this.context.device.Settings;
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let ethIntSettings = [];
        let ethIntState = [];
        let ethInt = [];
        let search = {_groupName: 'eth'};
        let settingsOptions = {
            sort: {_groupIndex: 1},
            fields: {name: 1, description: 1, 'ip_address': 1, _groupIndex: 1}
        };
        let stateOptions = {
            sort: {_groupIndex: 1},
            fields: {'admin_status': 1, oper_status: 1, uptime: 1}
        };

        let settingsSubscription = SubsManager.subscribe('settings', search, settingsOptions);
        let stateSubscription = SubsManager.subscribe('state', search, stateOptions);

        if (settingsSubscription.ready() && stateSubscription.ready()) {
            ethIntSettings = Settings.find(search, settingsOptions).fetch();
            ethIntState = State.find(search, stateOptions).fetch();

            ethIntSettings.forEach(function (elem, index, array) {
                let ethObj = {
                    settings: {},
                    state: {}
                };

                ethObj.settings = elem;
                ethObj.state = ethIntState[index];

                ethInt.push(ethObj);
            });
        }

        return {
            ethInt: ethInt
        };
    }

    renderInterfaces() {
        return (
            <table className="table table-striped">
                <thead>
                    <tr>
                    <th>Interface</th>
                    <th>Status</th>
                    <th>Description</th>
                    <th>IP Address</th>
                    <th></th>
                    </tr>
                </thead>
                <tbody>
                {this.data.ethInt.map(eth =>
                    <tr key={eth.settings._id}>
                        <td>eth{eth.settings._groupIndex + 1}</td>
                        <td>{eth.state['admin_status']}</td>
                        <td>{eth.settings['description']}</td>
                        <td>{eth.settings['ip_address']}</td>
                        <td>
                            <ContextualLink to={`management/ethernet/${eth.settings._groupIndex}`}>View/Edit</ContextualLink>
                        </td>
                    </tr>
                )}
                </tbody>
            </table>
        )
    }

    render() {
        return (
            <div style={{padding: '10px'}}>
                <h1>Ethernet Interfaces</h1>
                {this.data.ethInt.length > 0 ? this.renderInterfaces() : <p>Loading...</p>}
            </div>
        );
    }
}

reactMixin(EthernetList.prototype, ReactMeteorData);

EthernetList.contextTypes = {
    device: PropTypes.object.isRequired
};
