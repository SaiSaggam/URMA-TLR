import { UrmaServer, Collections } from 'meteor/digi:urma-core';
let log = new Logger('server/main');

Logger.setLevel('info');

let transportSettings = Assets.getText('transportSettings.xml');
let transportState = Assets.getText('transportState.xml');

Meteor.startup(function() {
    log.info('Starting Server ');
    UrmaServer.startup();
    // Load some test settings
    Collections.Settings.remove({});
    let rciSettings = UrmaServer.parseRciXml(transportSettings);
    UrmaServer.loadRci(Collections.Settings, rciSettings.settings, {});
    // Load some test state
    Collections.State.remove({});
    let rciState = UrmaServer.parseRciXml(transportState);
    UrmaServer.loadRci(Collections.State, rciState.state, {});
    // Load some test files
    Collections.Files.remove({});
    UrmaServer.loadFiles(Collections.Files);
});
