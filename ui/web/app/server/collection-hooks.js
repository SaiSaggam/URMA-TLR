import {Collections} from 'meteor/digi:urma-core';
import ccSimulator from './cellular-simulator.js';

let log = new Logger('server/collection-hooks');

let Settings = Collections.Settings;

Settings.after.update(function (userId, doc, fieldNames, modifier, options) {
    if(doc._groupName === "cellular"){
        log.debug('After updating cellular settings');
        ccSimulator.simulateCellularConnection();
    }
}, {fetchPrevious: false});
