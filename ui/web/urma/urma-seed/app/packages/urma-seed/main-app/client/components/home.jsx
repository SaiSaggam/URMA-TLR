import { Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';

let Link = Urma.ContextualLink;
let Authorized = Urma.Authorized;

export default class Home extends Component {

    getMeteorData() {
        let State = this.context.State;
        let SubsManager = this.context.SubsManager;

        let deviceInfo = {};
        let deviceStats = {};
        let diSearch = {_groupName: 'device_info'};
        let dsSearch = {_groupName: 'device_stats'};
        let diSubscription = SubsManager.subscribe('state', diSearch);
        let dsSubscription = SubsManager.subscribe('state', dsSearch);
        if (diSubscription.ready() && dsSubscription.ready()) {
            deviceInfo = State.findOne(diSearch);
            deviceStats = State.findOne(dsSearch);
        }

        return {
            deviceInfo: deviceInfo,
            deviceStats: deviceStats
        };
    }

    render() {
        return (
            <div className='home-container'>
                <div className='home-panel panel-full-width'>
                    <h4>Device Overview</h4>
                    <div className='home-panel-content'>
                        <div className='panel-wide'>
                            <img src='/images/device.png' alt=''/>
                            <ul>
                                <div className='panel-wide--half'>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o status-green'></i>
                                        <span>Power</span>
                                    </li>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o'></i>
                                        <span>Service</span>
                                    </li>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o status-alert'></i>
                                        <span>WWAN</span>
                                    </li>
                                </div>
                                <div className='panel-wide--half'>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o'></i>
                                        <span>Signal 1</span>
                                    </li>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o status-green'></i>
                                        <span>Signal 2</span>
                                    </li>
                                    <li className='home-panel-content-list-item'>
                                        <i className='status-indicator fa fa-dot-circle-o'></i>
                                        <span>Signal 3</span>
                                    </li>
                                </div>
                            </ul>
                        </div>
                        <div className='panel-wide'>
                            <div className='home-page-content-header'>
                                <h5>Info</h5>
                                <div className='home-page-content-section'>
                                    {this.data.deviceInfo._id ?
                                        <ul>
                                            <li className='home-panel-content-list-item'>
                                                Model: {this.data.deviceInfo.model}</li>
                                            <li className='home-panel-content-list-item'>Firmware
                                                Version: {this.data.deviceInfo.firmware}</li>
                                            <li className='home-panel-content-list-item'>Boot
                                                Version: {this.data.deviceInfo.boot}</li>
                                        </ul>
                                        :
                                        <p>Loading...</p>
                                    }
                                </div>
                            </div>

                            <Authorized>
                                <div className='home-page-content-header'>
                                    <h5>Status</h5>
                                    <div className='home-page-content-section'>
                                        {this.data.deviceStats._id ?
                                            <ul>
                                                <li className='home-panel-content-list-item'>CPU
                                                    Utilization: {this.data.deviceStats.cpu} %
                                                </li>
                                                <li className='home-panel-content-list-item'>Temperature
                                                    (CPU): {this.data.deviceStats.coretemp} &#0176; F
                                                </li>
                                            </ul>
                                            :
                                            <p>Loading...</p>
                                        }
                                    </div>
                                </div>
                            </Authorized>
                        </div>
                    </div>
                </div>
                <Authorized>
                    <div className='home-panel panel-medium'>
                        <h4>Interfaces</h4>
                        <div className='home-panel-content'>
                            <div className='btn-group'>
                                <button type='button' className='btn button-colors-common'>Ethernet</button>
                            </div>
                            <div className='home-page-content-header'>
                                <h5>Ethernet Settings</h5>
                                <div className='home-page-content-section'>
                                    <div className='btn-group'>
                                        <button type='button' className='btn button-colors-common'>Ethernet 1</button>
                                    </div>
                                    <ul className='nested-list'>
                                        <li className='home-panel-content-list-item'>IP Address: 192.168.1.1</li>
                                        <li className='home-panel-content-list-item'>Primary DNS Server: 192.168.1.1
                                        </li>
                                        <li className='home-panel-content-list-item'>Secondary DNS Server: 192.168.1.1
                                        </li>
                                        <li className='home-panel-content-list-item'>Mask: 255.255.255.0</li>
                                        <li className='home-panel-content-list-item'>Gateway: 10.80.1.1</li>
                                    </ul>
                                </div>
                            </div>
                        </div>
                        <div className='home-panel-action'>
                            <a ui-sref='device-config.ethernet'>Configure Ethernet Settings</a>
                        </div>
                    </div>
                    <div className='home-panel panel-narrow'>
                        <h4>Operations</h4>
                        <div className='home-panel-content'>
                            <div className='home-page-content-header'>
                                <h5>Firmware Update Scheduled<span
                                    className='pull-right status-alert glyphicon glyphicon-exclamation-sign'></span>
                                </h5>
                                <div className='home-page-content-section'>
                                    <p>9/12/2015 8:00 A.M.</p>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div className='home-panel panel-narrow'>
                        <h4>Alarms</h4>
                        <div className='home-panel-content'>
                            <div className='home-page-content-header'>
                                <h5>High Temperature<span
                                    className='pull-right status-alert glyphicon glyphicon-exclamation-sign'></span>
                                </h5>
                                <div className='home-page-content-section'>
                                    <p>Fired: 9/03/2015 10:00 A.M.</p>
                                </div>
                            </div>
                        </div>
                    </div>
                </Authorized>
            </div>
        );
    }
}

reactMixin(Home.prototype, ReactMeteorData);

Home.contextTypes = {
    SubsManager: React.PropTypes.object.isRequired,
    State: React.PropTypes.object.isRequired
};
