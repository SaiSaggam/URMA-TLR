// Write your package code here!
import mainRoutes from './main-app/client/index.js';
import gettingStartedRoutes from './getting-started/index.jsx';
import Urma from 'meteor/digi:urma-core';

const DeviceUIInfo = {
    devicePath: 'LR54DWC1',
    mainRoutes: mainRoutes,
    gettingStartedRoutes: gettingStartedRoutes
};

Urma.DeviceUIStore.push(DeviceUIInfo);

export default DeviceUIInfo;
