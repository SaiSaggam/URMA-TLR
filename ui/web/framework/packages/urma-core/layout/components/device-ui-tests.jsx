import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let routerHelper = new Urma.RouterHelper();
let DeviceUI = Urma.DeviceUI;
let Navigation = Urma.Navigation;
let Header = Urma.Header;
let MainContent = UrmaTest.MainContent;
let ContextualLink = Urma.ContextualLink;

describe('DeviceUI Component Tests', function () {
    let deviceCtx = {
        env: 'device',
        deviceID: '123',
        SubsManager: null,
        Settings: {},
        State: {},
        Files: {}
    };

    class Parent extends React.Component {
        getChildContext() {
            return {
                device: deviceCtx
            };
        }

        render() {
            //return <div>parent {this.props.children}</div>
            return (
                <DeviceUI title="DeviceUI unit test"
                          logo="/image/mylogo.png"
                          routes={routes}
                          location={this.props.location}
                          children={this.props.children}
                />
            )
        }
    }
    Parent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    class Child extends React.Component {
        render() {
            return <div>child</div>
        }
    }

    let routes = {
        path: '/',
        component: Parent,
        public: true,
        indexRoute: {
            name: 'child',
            component: Child,
            public: true,
            nav: {
                menuLabel: 'Child'
            }
        }
    };


    beforeEach(function () {
        // Set default environment for device context.
        deviceCtx.env = 'device';
        // Setup routes for each test

        routerHelper.configureRoutes(routes, '/')
    });

    afterEach(function () {
        //clear routes by resetting mainMenuItems array
        routerHelper._mainMenuItems = [];
    });

    it('should render layout components with expected properties', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});

        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);

        let deviceUI = TestUtils.findRenderedComponentWithType(root, DeviceUI);
        let header = TestUtils.findRenderedComponentWithType(deviceUI, Header);
        let navigation = TestUtils.findRenderedComponentWithType(deviceUI, Navigation);
        let content = TestUtils.findRenderedComponentWithType(deviceUI, MainContent);

        expect(deviceUI).to.exist;
        expect(header).to.exist;
        expect(header.props).to.have.property('title', 'DeviceUI unit test');
        expect(header.props).to.have.property('logo', '/image/mylogo.png');
        expect(header.props).to.have.property('loginService');
        expect(navigation).to.exist;
        expect(navigation.props).to.have.all.keys(['setOpenState', 'openStateClass', 'openState', 'routes']);
        expect(content).to.exist;
        expect(content.props).to.have.all.keys(['openStateClass', 'children']);
    }));

    it('should show all components when user logged in', sinon.test(function () {

        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});

        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);

        let deviceUI = TestUtils.findRenderedComponentWithType(root, DeviceUI);
        let header = TestUtils.findRenderedComponentWithType(deviceUI, Header);
        let navigation = TestUtils.findRenderedComponentWithType(deviceUI, Navigation);
        let content = TestUtils.findRenderedComponentWithType(deviceUI, MainContent);
        let unauthorizedDivs = TestUtils.scryRenderedDOMComponentsWithClass(deviceUI, 'unauthorized');

        expect(deviceUI).to.exist;
        expect(header).to.exist;
        expect(navigation).to.exist;
        expect(content).to.exist;
        expect(unauthorizedDivs.length).to.equal(0);
    }));

    it.skip('should show hide navigation menu when user logged out', sinon.test(function () {
        //skipping for now because we're redirecting to getting started wizard if user is logged out.

        let stubUser = this.stub(Meteor, "user", ()=> {return null});

        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);

        let deviceUI = TestUtils.findRenderedComponentWithType(root, DeviceUI);
        let header = TestUtils.findRenderedComponentWithType(deviceUI, Header);
        let navigation = TestUtils.findRenderedComponentWithType(deviceUI, Navigation);
        let content = TestUtils.findRenderedComponentWithType(deviceUI, MainContent);
        let outerDiv = TestUtils.findRenderedDOMComponentWithClass(deviceUI, 'unauthorized');

        expect(deviceUI).to.exist;
        expect(header).to.exist;
        expect(navigation).to.exist;
        expect(content).to.exist;
        expect(outerDiv).to.exist;
    }));

    it('should not render logo and login buttons when running in DRM environment', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});

        deviceCtx.env = 'drm';
        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);

        let deviceUI = TestUtils.findRenderedComponentWithType(root, DeviceUI);
        let header = TestUtils.findRenderedComponentWithType(deviceUI, Header);
        let navigation = TestUtils.findRenderedComponentWithType(deviceUI, Navigation);
        let content = TestUtils.findRenderedComponentWithType(deviceUI, MainContent);

        expect(deviceUI).to.exist;
        expect(header).to.exist;
        expect(header.props).to.have.property('title', 'DeviceUI unit test');
        expect(header.props).to.have.property('logo', null);
        expect(header.props).to.have.property('loginService', null);
        expect(navigation).to.exist;
        expect(navigation.props).to.have.all.keys(['setOpenState', 'openStateClass', 'openState', 'routes']);
        expect(content).to.exist;
        expect(content.props).to.have.all.keys(['openStateClass', 'children']);
    }));

    it('should build basePath from deviceId and devicePath', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});

        class Parent extends React.Component {
            getChildContext() {
                return {
                    device: deviceCtx
                };
            }

            render() {
                //return <div>parent {this.props.children}</div>
                return (
                    <DeviceUI title="DeviceUI unit test"
                        logo="/image/mylogo.png"
                        routes={childRoute}
                        location={this.props.location}
                        children={this.props.children}
                    />
                )
            }
        }
        Parent.childContextTypes = {
            device: React.PropTypes.object.isRequired
        };

        class Index extends React.Component {
            render() {
                return <div>index</div>
            }
        }

        class Child extends React.Component {
            render() {
                return <div>child</div>
            }
        }



        let childRoute = {
            path: 'TLR',
            public: true,
            indexRoute: {
                name: 'child',
                component: Child,
                public: true,
                nav: {
                    menuLabel: 'Child'
                }
            }
        }

        let routes = {
            path: '/',
            component: Parent,
            public: true,
            indexRoute: {
                name: 'child',
                component: Index,
                public: true,
                nav: {
                    menuLabel: 'Child'
                }
            },
            childRoutes: [childRoute]
        };



        deviceCtx.env = 'drm';

        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        let ctxlink = TestUtils.findRenderedComponentWithType(root, ContextualLink);
        let link = TestUtils.findRenderedDOMComponentWithTag(ctxlink, 'a');

        expect(link.getAttribute('href')).to.equal('/devices/123/TLR/');
    }));

    it('should set basePath to root', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});

        deviceCtx.env = 'device';

        let root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        let ctxlink = TestUtils.findRenderedComponentWithType(root, ContextualLink);
        let link = TestUtils.findRenderedDOMComponentWithTag(ctxlink, 'a');

        expect(link.getAttribute('href')).to.equal('/');

    }));
});
