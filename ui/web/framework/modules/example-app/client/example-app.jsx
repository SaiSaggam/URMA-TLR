//TODO: Move header to main container
import { Router, Route, Link } from 'react-router';
import React from 'react';
import Urma from 'meteor/digi:urma-core';
import {Collections} from 'meteor/digi:urma-core';

let DeviceUI = Urma.DeviceUI;

export default class ExampleApp extends React.Component {
    getChildContext() {
        let deviceCtx = {
            env: 'device',
            deviceID: null,
            SubsManager: Urma.SubsManager,
            Settings: Collections.Settings,
            State: Collections.State,
            Files: Collections.Files
        };
        return {
            device: deviceCtx
        };
    }

    render() {
        return (
            <DeviceUI title='URMA Framework Guide'
                      location={this.props.location}
                      children={this.props.children}
            />
        );
    }
}

ExampleApp.childContextTypes = {
    device: React.PropTypes.object.isRequired
};
