import { UrmaServer, Collections } from 'meteor/digi:urma-core';

let transportSettings = Assets.getText('transportSettings.xml');
let transportState = Assets.getText('transportState.xml');

Meteor.startup(function() {
    console.log('Starting URMA Framework Example App Server ');
    UrmaServer.startup();
    // Load some test data
    Collections.Settings.remove({});
    let rciSettings = UrmaServer.parseRciXml(transportSettings);
    UrmaServer.loadRci(Collections.Settings, rciSettings.settings);
    Collections.State.remove({});
    let rciState = UrmaServer.parseRciXml(transportState);
    UrmaServer.loadRci(Collections.State, rciState.state);
    Collections.Files.remove({});
    UrmaServer.loadFiles(Collections.Files);
});
