import React from 'react';
import ReactDOM from 'react-dom';
import {Router, browserHistory} from 'react-router';
import MainApp from './MainApp.jsx';
import DeviceUIInfo from 'meteor/digi:urma-tlr';

let mainRoutes = DeviceUIInfo.mainRoutes;
let gettingStartedRoutes = DeviceUIInfo.gettingStartedRoutes;

//-------------------------------------------------------------------------------
// Pulled the following code from ReactRouterSSR with some slight modifications.
// Switched to using the meteor reactrouter:react-router package instead of the
// NPM react-router package. Although now that it is working can probably figure
// out how to switch back.
// Need to figure out the *right* way to do this!
//-------------------------------------------------------------------------------
const routes = {
    component: MainApp,
    childRoutes: [
        gettingStartedRoutes,
        mainRoutes
    ]
};

Meteor.startup(function() {
    const rootElementName = 'react-app';
    const rootElementType = 'div';
    const attributes = [];
    let rootElement = document.getElementById('react-app');

    // In case the root element doesn't exist, let's create it
    if (!rootElement) {
        rootElement = document.createElement(rootElementType);
        rootElement.id = rootElementName;

        // check if a 2-dimensional array was passed... if not, be nice and handle it anyway
        if (attributes[0] instanceof Array) {
            // set attributes
            for (var i = 0; i < attributes.length; i++) {
                rootElement.setAttribute(attributes[i][0], attributes[i][1]);
            }
        } else if (attributes.length > 0) {
            rootElement.setAttribute(attributes[0], attributes[1]);
        }

        document.body.appendChild(rootElement);
    }

    let app = (
        <Router
            history={browserHistory}
            children={routes}/>
    );

    ReactDOM.render(app, rootElement);
});
