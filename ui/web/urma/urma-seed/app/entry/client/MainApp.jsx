import { Component } from 'react';
import Helmet from 'react-helmet';
import {Settings, State, Files} from 'meteor/digi:urma-seed';

let SubsManager = Urma.SubsManager;

export default class MainApp extends Component {
    getChildContext() {
        return {
            env: 'device',
            deviceID: null,
            deviceType: null,
            SubsManager: SubsManager,
            Settings: Settings,
            State: State,
            Files: Files
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
    env: React.PropTypes.string.isRequired,
    deviceID: React.PropTypes.string,
    deviceType: React.PropTypes.string,
    SubsManager: React.PropTypes.object.isRequired,
    Settings: React.PropTypes.object.isRequired,
    State: React.PropTypes.object.isRequired,
    Files: React.PropTypes.object.isRequired
};