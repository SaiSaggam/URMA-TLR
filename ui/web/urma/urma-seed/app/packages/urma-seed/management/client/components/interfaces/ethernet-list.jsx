import { Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';

let Link = Urma.ContextualLink;

export default class EthernetList extends Component {

    getMeteorData() {
        let Settings = this.context.Settings;
        let State = this.context.State;
        let SubsManager = this.context.SubsManager;

        let ethIntSettings = [];
        let ethIntState = [];
        let ethInt = [];
        let settingsSearch = {_groupName: 'ethernet'};
        let stateSearch = {_groupName: 'eth'};
        let settingsOptions = {
            sort: {_groupIndex: 1},
            fields: {name: 1, desc: 1, IPaddr: 1, _groupIndex: 1}
        };
        let stateOptions = {
            sort: {_groupIndex: 1},
            fields: {admin_status: 1, oper_status: 1, uptime: 1}
        };

        let settingsSubscription = SubsManager.subscribe('settings', settingsSearch, settingsOptions);
        let stateSubscription = SubsManager.subscribe('state', stateSearch, stateOptions);

        if (settingsSubscription.ready() && stateSubscription.ready()) {
            ethIntSettings = Settings.find(settingsSearch, settingsOptions).fetch();
            ethIntState = State.find(stateSearch, stateOptions).fetch();

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
                        <td>{eth.state.admin_status}</td>
                        <td>{eth.settings.desc}</td>
                        <td>{eth.settings.IPaddr}</td>
                        <td>
                            <Link to={`/management/ethernet/${eth.settings._groupIndex}`}>View/Edit</Link>
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
    SubsManager: React.PropTypes.object.isRequired,
    Settings: React.PropTypes.object.isRequired,
    State: React.PropTypes.object.isRequired
};
