import {default as React, Component, PropTypes } from 'react';

export default class EthernetState extends Component {
    render() {
        return (
            <div className="group">
                <div className={this.props.openState ? 'toggle-group open' : 'toggle-group'}>
                    <p><span className="bold">Configured Port Status: </span>{this.props.ethState['admin_status']}</p>

                    <p><span className="bold">Operational Port Status: </span>{this.props.ethState['oper_status']}</p>

                    <p><span className="bold">Uptime in Seconds: </span>{this.props.ethState['uptime']}</p>
                </div>
            </div>
        )
    }
}
