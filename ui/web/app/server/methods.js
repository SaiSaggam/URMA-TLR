import {Collections} from 'meteor/digi:urma-core';
import fwUpdateSimulator from './firmware-update-simulator.js';

let log = new Logger('server/methods');

let State = Collections.State;

Meteor.methods({
    updateFirmware: function(filename, options) {
        log.debug('Applying firmware file '+filename+' to the device. options:', options);
        fwUpdateSimulator.simulateFirmwareUpdate(filename, options);
    },
    reboot: function(delay) {
        log.debug('Rebooting the device in '+delay+' minutes...');
        //TODO: simulate reboot - can we close the DDP connection?
    },
    resetUsernamePwd: function (userId, username, password) {
        if(username && username.length) {
            Accounts.setUsername(userId, username);
        }
        if(password && password.length) {
            Accounts.setPassword(userId, password, {logout: false});
        }
    },
    provisionDevice: function(username, password, deviceId, server) {
        log.debug('Provision device with device id ' + deviceId + 'to device cloud server ' + server + ', using username: ', username);
        let auth = 'Basic ' + new Buffer(username + ':' + password).toString('base64');
        let cloudUrl = "https://" + server+"/ws/v1/devices/inventory" ;
        let httpConn, error, result ;
        let options = {
            data: {},
            headers: {
                "Authorization": auth,
                "content-type": "application/json",
                "Accept": "application/json"
            }
        };

        try{
            /** first check to see if device is already provisioned **/
            httpConn =  HTTP.get(cloudUrl+ "/" +deviceId, options);
            if (httpConn.statusCode == 200 || httpConn.statusCode == 201) {
                result = httpConn.statusCode;
            }
        }catch(e) {
            if (e.response.statusCode == 404) {
                try {
                    options.data = {"id": deviceId} ;
                    httpConn = HTTP.post(cloudUrl,options);
                    if (httpConn.statusCode == 200 || httpConn.statusCode == 201) {
                        /** successfully provisioned... return success **/
                        result = httpConn.statusCode;
                    } else {
                        /** something went wrong on the provision call, return what we know **/
                        error = new Meteor.Error(httpConn.statusCode, "Unable to provision the device.");
                    }
                } catch (e) {
                    error = new Meteor.Error(e.response.statusCode, "Failed to provision the device.");
                }
            } else {
                error = new Meteor.Error(e.response.statusCode, "Unable to read the device list.");
            }
        }

        return {
            'error': error,
            'result': result
        }
    }
});