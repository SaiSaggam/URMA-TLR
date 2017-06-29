import React from 'react';
import {Link} from 'react-router';

export default class ContextualLink extends React.Component {
    constructor(props) {
        super(props);
    }

    getPath(){
        let basePath = this.context.basePath;
        let path = this.props.to;
        
        if (basePath !== '/' && path === this.context.devicePath || path === '/') {
            //our route is the index route.
            return basePath;
        } else {
            return basePath + path;
        }
    }

    render(){
        // NOTE: order matters with the spread attributes (...this.props) in the Link component.
        // Spread should come first so that our 'to' property overrides what is in this.props.
        return (
            <Link{...this.props} to={this.getPath()}>{this.props.children}</Link>
        )
    }
}

// basePath is defined in the DeviceUI component (urma-core) and is formed from
// device.deviceID context passed down from the Device component (in the app) and
// the index route path
ContextualLink.contextTypes = {
    basePath: React.PropTypes.string.isRequired,
    devicePath: React.PropTypes.string.isRequired
};
