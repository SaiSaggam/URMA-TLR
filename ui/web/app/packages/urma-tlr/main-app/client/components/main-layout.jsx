import {default as React, Component, PropTypes} from 'react';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';
import routes from '../routes.jsx';
let DeviceUI = Urma.DeviceUI;

export default class MainLayout extends Component {

    getMeteorData() {
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let title = '';
        let search = {_groupName: 'system'};
        let subscription = SubsManager.subscribe('state', search);
        if (subscription.ready()) {
            let system = State.findOne(search);
            title = system.model || '';
        }

        return {
            title: title
        };
    }

    render() {
        return (
            <DeviceUI title={this.data.title}
                      logo='/images/logo.png'
                      routes={routes}
                      location={this.props.location}
                      children={this.props.children}
            />
        );
    }
}

reactMixin(MainLayout.prototype, ReactMeteorData);

MainLayout.contextTypes = {
    device: PropTypes.object.isRequired
};
