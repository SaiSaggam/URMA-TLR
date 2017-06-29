import Urma from 'meteor/digi:urma-core';
import FWUpdate from './components/firmware-update.jsx';
import Login from './components/login.jsx';
import getStarted from './components/get-started.jsx';
import DRMConnect from './components/DRMconnect.jsx';
import Connect from './components/connect.jsx';
import Cellular from './components/cellular.jsx';
import CellularResults from './components/cellular-results.jsx';
import ChangePassword from './components/change-password.jsx';
import Welcome from './components/welcome.jsx';

let Wizard = Urma.Wizard;

export default {
    name: 'getting-started',
    path: 'getting-started',
    component: Wizard,
    indexRoute: {
        name: 'welcome',
        public: true,
        component: Welcome
    },
    childRoutes: [
        {
            name: 'welcome',
            path: 'welcome',
            public: true,
            component: Welcome
        },
        {
            name: 'login',
            path: 'login',
            public: true,
            component: Login
        },
        {
            name: 'change-password',
            path: 'change-password',
            component: ChangePassword
        },
        {
            name: 'connect',
            path: 'connect',
            component: Connect
        },
        {
            name: 'cellular',
            path: 'cellular/:_groupIndex',
            component: Cellular
        },
        {
            name: 'ethernet',
            path: 'ethernet',
            component: getStarted.Ethernet
        },
        {
            name: 'cellular-results',
            path: 'cellular-results',
            component: CellularResults
        },
        {
            name: 'firmware-update',
            path: 'firmware-update',
            component: FWUpdate
        },
        {
            name: 'remote-manager-connect',
            path: 'remote-manager-connect',
            component: DRMConnect
        },
        {
            name: 'about-dashboard',
            path: 'about-dashboard',
            component: getStarted.AboutDashboard
        }
    ]
};
