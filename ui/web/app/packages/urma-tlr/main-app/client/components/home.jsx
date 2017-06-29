import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';

let Link = Urma.ContextualLink;
let Authorized = Urma.Authorized;

export default class Home extends Component {

    getMeteorData() {
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let system = {};
        let search = {_groupName: 'system'};
        let subscription = SubsManager.subscribe('state', search);
        if (subscription.ready()) {
            system = State.findOne(search);
        }

        return {
            system: system
        };
    }

    render() {
        return (
            <div className='home-container'>
                <div className='home-panel panel-full-width'>
                    <h4>Device Overview</h4>
                    <div className='home-panel-content'>
                        <div className='panel-wide'>
                            <img src='/images/LR_frontPanel.jpg'/>
                            
                        </div>
                        <div className='panel-wide'>
                            <div className='home-page-content-header'>
                                <h5>Info</h5>
                                <div className='home-page-content-section'>
                                    {this.data.system._id ?
                                        <ul>
                                            <li className='home-panel-content-list-item'>
                                                Model: {this.data.system['model']}</li>
                                            <li className='home-panel-content-list-item'>Firmware
                                                Version: {this.data.system['firmware_version']}</li>
                                            <li className='home-panel-content-list-item'>Boot
                                                Version: {this.data.system['bootloader_version']}</li>
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
                                        {this.data.system._id ?
                                            <ul>
                                                <li className='home-panel-content-list-item'>CPU
                                                    Utilization: {this.data.system['cpu_usage']} %
                                                </li>
                                                <li className='home-panel-content-list-item'>Temperature
                                                    (CPU): {this.data.system['temperature']} &#0176; F
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
            </div>
        );
    }
}

reactMixin(Home.prototype, ReactMeteorData);

Home.contextTypes = {
    device: PropTypes.object.isRequired
};
