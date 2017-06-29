import React from 'react';
import reactMixin from 'react-mixin';
import Header from './header.jsx';
import MainContent from './main-content.jsx';
import Navigation from '../../navigation/components/navigation.jsx';
import { DeviceLoginService } from '../../security/device-login';

/**
 * Main component for the device UI. Provides the overall page layout with header, navigation menu, and page content
 * container.
 * @type {DeviceUI}
 */
export default class DeviceUI extends React.Component {
    constructor(props){
        super(props);
        this.state = {
            open: true
        };
    }

    getMeteorData() {
        return {
            user: Meteor.user()
        }
    }

    getChildContext() {
        if (this.context.device.env === 'drm') {
            return {
                basePath: '/devices/' + this.context.device.deviceID + '/' + this.props.routes.path + '/',
                devicePath: this.props.routes.path
            };
        } else {
            return {
                basePath: '/',
                devicePath: this.props.routes.path
            };
        }
    }

    componentWillMount() {
        // Ensure if we are not logged in when mounting we navigate back to home (root) page
        if (!this.data.user && this.props.location) {
            //TODO: change this back to send to index for 1.1
            this.context.router.push(`/getting-started`);
        }
    }

    componentDidUpdate(prevProps, prevState) {
        // When data changes ensure if user logged out we navigate to home (root) page
        if (!this.data.user && this.props.location) {
            //TODO: change this back to send to index for 1.1
            this.context.router.push(`/getting-started`);
        }
    }

    //set open state that is used by navigation and main container to determine width and link display.
    setOpenState(elem){
        if (elem === 'menu-item') {
            // Clicked on a menu item. Open if not currently open.
            if(!this.state.open) {
                this.setState({open: true});
            }
        } else {
            // Clicked on navigation expand/collapse button. Toggle open state
            this.setState({open: !this.state.open});
        }
    }

    openStateClass(){
        if(this.state.open){
            return 'open';
        } else {
            return 'collapsed'
        }
    }

    render(){
        let outerClass = this.data.user ? '' : 'unauthorized';
        // When loaded from within DRM we don't want to show the logo or login controls. DRM
        // provides them in the outer application.
        let isDRM = (this.context.device && this.context.device.env === 'drm');
        let logo = isDRM ? null : this.props.logo;
        let loginService = isDRM ? null : DeviceLoginService;

        return (
            <div className={outerClass}>
                <Header title={this.props.title} logo={logo} loginService={loginService} />
                <Navigation routes={this.props.routes} setOpenState={this.setOpenState.bind(this)} openStateClass={this.openStateClass.bind(this)} openState={this.state.open}/>

                <MainContent openStateClass={this.openStateClass.bind(this)} children={this.props.children}/>
            </div>
        );
    }
};


DeviceUI.propTypes = {
    location: React.PropTypes.object.isRequired,  // location object from react router context
    children: React.PropTypes.object.isRequired,   // route children
    title: React.PropTypes.string.isRequired,
    routes: React.PropTypes.object.isRequired,
    logo: React.PropTypes.string
};

//device.deviceID context passed down from the Device component (in the app)
DeviceUI.contextTypes = {
    router: React.PropTypes.object.isRequired,
    device: React.PropTypes.object.isRequired
};

//this context is used in the contextual link component (urma-core) and in Navigation component (urma-core)
DeviceUI.childContextTypes = {
    basePath: React.PropTypes.string.isRequired,
    devicePath: React.PropTypes.string.isRequired
};

reactMixin(DeviceUI.prototype, ReactMeteorData);