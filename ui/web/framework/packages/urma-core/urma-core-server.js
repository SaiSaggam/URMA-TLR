// Collections
import Settings from './collections/settings.js';
import State from './collections/state.js';
import Files from './collections/files.js';
import DevicesLoaded from './collections/devices-loaded.js'

/**
 * Core collections for URMA devices
 * @type {{Settings: Mongo.Collection, State: Mongo.Collection, Files: Mongo.Collection}}
 */
export const Collections =  {
    Settings: Settings,
    State: State,
    Files: Files,
    DevicesLoaded: DevicesLoaded
};

// Re-export UrmaServer
export * from './collections/server/main.js';
