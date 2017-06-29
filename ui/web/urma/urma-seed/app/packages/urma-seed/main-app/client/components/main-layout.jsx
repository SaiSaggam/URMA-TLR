import { Component, PropTypes } from 'react';

let DeviceUI = Urma.DeviceUI;

export default class MainLayout extends Component {

    render(){
        return (
            <DeviceUI title='TransPort WR21'
                      logo='/images/logo.png'
                      location={this.props.location}
                      history={this.props.history}
                      children={this.props.children}
            />
        );
    }
}
