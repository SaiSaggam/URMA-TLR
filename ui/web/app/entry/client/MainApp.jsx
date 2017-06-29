import {default as React, Component, PropTypes} from 'react';
import Helmet from 'react-helmet';
import Urma from 'meteor/digi:urma-core';
import {Collections} from 'meteor/digi:urma-core';

let SubsManager = Urma.SubsManager;
let Settings = Collections.Settings;
let State = Collections.State;
let Files = Collections.Files;

Logger.setLevel('info');

export default class MainApp extends Component {
    getChildContext() {
        let deviceCtx = {
            env: 'device',
            deviceID: null,
            deviceType: 'TLR',
            SubsManager: SubsManager,
            Settings: Settings,
            State: State,
            Files: Files
        };
        return {
            device: deviceCtx
        };
    }

    render() {
        return (
            <div>
                <Helmet
                    meta={[
                        { name: 'viewport', content: 'width=device-width, initial-scale=1' }
                    ]}
                />
                {this.props.children}
            </div>
        )
    }
}

MainApp.childContextTypes = {
    device: PropTypes.object.isRequired
};
