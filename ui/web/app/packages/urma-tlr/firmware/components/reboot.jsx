import React from 'react';
import { HTTP } from 'meteor/http';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';

let log = new Logger('firmware/reboot');

let reqInterval = null;
let timeOut = null;
let reconnected = false;

const REBOOT_TIMEOUT = 180000;

const LOADING = 1; //initial state
const REBOOTING = 2;
const CONNECTED = 3;
const READY = 4;

let CountdownTimer = Urma.CountdownTimer;

let mSettings = (Meteor.settings && Meteor.settings.public ? Meteor.settings.public : {});
let developmentMode = mSettings.development || false;


export default class Reboot extends React.Component {
    constructor(props){
        super(props);
        this.rebootStatus = LOADING;
        this.state = {
            timedOut: false,
            status: LOADING
        };
    }

    getMeteorData() {
        log.debug('reboot meteor status update, connected:', Meteor.status().connected);
        this.updateRebootStatus(Meteor.status().connected, false);

        return {
            status: Meteor.status()
        };
    }

    componentWillMount(){
        log.debug('reboot component mounted, reboot status:', this.rebootStatus);

        if(developmentMode) {
            Urma.Simulators.SimulateReboot.simulateReboot();
        }

        // test when server comes back up on a regular interval
        reqInterval = setInterval(this.isServerUp.bind(this), 10000);

        // show time out error in case we are unable to reestablish a connection in 3 minutes.
        timeOut = setTimeout(this.onTimeout.bind(this), REBOOT_TIMEOUT);
    }

    componentWillUnmount(){
        if(developmentMode) {
            Urma.Simulators.SimulateReboot.cleanUp();
        }

        this.clearTimers();
    }

    onTimeout() {
        this.setState({timedOut: true});
        if (reqInterval != null) {
            clearInterval(reqInterval);
            reqInterval = null;
        }
    }

    onReconnect() {
        if (reconnected) {
            log.debug('ignoring onReconnect')
        } else {
            log.debug('onReconnect called, set timer to redirect');
            reconnected = true;
            this.clearTimers();
            setTimeout(()=>{
                window.location.assign('/');
            }, 10000);
        }
    }

    clearTimers() {
        if (reqInterval != null) {
            clearInterval(reqInterval);
            reqInterval = null;
        }
        if (timeOut != null) {
            clearTimeout(timeOut);
            timeOut = null;
        }
    }

    //polling server every 3 secs in case socket closes before device comes back up
    isServerUp() {
        HTTP.get('/images/logo.png?d='+Date.now(), {}, (err, results) => {
            let connected = !err;
            log.debug('http server check, connected:', connected);
            this.updateRebootStatus(connected, true);
            this.setState({status: this.rebootStatus}); // Force update/render
        })
    }

    updateRebootStatus(connected, ready) {
        log.debug('update reboot status:', this.rebootStatus, ', ready:', ready);
        if(!connected && this.rebootStatus === LOADING){
            //update reboot status from initial value of LOADING to REBOOTING when we detect
            //that we have lost the connection (DDP connection or HTTP GET polling).
            this.rebootStatus = REBOOTING;
            log.debug('-> updated from loading (1) to:', this.rebootStatus);
        } else if (connected && (this.rebootStatus === REBOOTING || this.rebootStatus === CONNECTED)) {
            //update reboot status from disconnected value of REBOOTING to CONNECTED or READY when we detect
            //that we have reestablished the connection (DDP connection or HTTP GET polling).
            this.rebootStatus = ready ? READY : CONNECTED;
            log.debug('-> updated from rebooting (2) to:', this.rebootStatus);
        }

    }

    render() {
        if(this.rebootStatus === READY) {
            log.debug('calling onReconnect from render, reboot status:', this.rebootStatus);
            //run onReconnect if we have disconnected and reconnected.
            this.onReconnect();
        }

        return (
            <div>
                {!this.state.timedOut ?
                    <div className="reboot-msg">
                            {this.rebootStatus === CONNECTED || this.rebootStatus === READY ?
                                <div>
                                    <h5>Device connected. Initializing...</h5>
                                </div>
                                :
                                <div>
                                    <h5>Please wait while your device reboots.</h5>
                                    <CountdownTimer timeRemaining={REBOOT_TIMEOUT}/>
                                </div>
                            }
                        <div className="loader">Loading...</div>
                    </div>
                    :
                    <div>
                        <h5 className="error">Timed out while waiting for device to reconnect.</h5>
                    </div>
                }

            </div>
        );
    }
};

reactMixin(Reboot.prototype, ReactMeteorData);
