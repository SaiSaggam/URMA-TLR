import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let Navigation = Urma.Navigation;
let RouterHelper = new Urma.RouterHelper();
let fakeSetOpenState = sinon.spy();
let fakeOpenStateClass = sinon.spy();
let fakeInitState = false;

describe('Navigation Tests', function () {
    let root, nav;

    class Wrapper extends React.Component {
        getChildContext() {
            return {
                basePath: '/',
                devicePath: 'TLR'
            };
        }
        render() {
            return (
                <Navigation
                    routes={routes}
                    setOpenState={fakeSetOpenState}
                    openState={fakeInitState}
                    openStateClass={fakeOpenStateClass}/>

            )
        }
    }
    Wrapper.childContextTypes = {
        basePath: React.PropTypes.string.isRequired,
        devicePath: React.PropTypes.string.isRequired
    };

    let routes = {
        path: '/',
        component: Wrapper,
        public: true,
        indexRoute: {
            name: 'home',
            public: true,
            nav: {
                menuLabel: 'Home',
                order: 0
            }
        },
        childRoutes: [
            {
                name: 'security',
                path: '/security',
                public: true,
                nav: {
                    menuLabel: 'Security',
                    order: 0
                }
            }
        ]
    };

    beforeEach(function () {
        RouterHelper.configureRoutes(routes, '/');

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory}/>);
        nav = TestUtils.findRenderedComponentWithType(root, Navigation);

    });

    afterEach(function () {
        RouterHelper._mainMenuItems = [];
    });

    it('should contain navigation element', sinon.test(function () {
        let menuEl = TestUtils.findRenderedDOMComponentWithClass(nav, 'config-nav-main');
        expect(menuEl).to.not.be.undefined;
    }));

    it('should handle click', sinon.test(function () {
        let link = TestUtils.findRenderedDOMComponentWithClass(nav, 'main-menu-toggle-link');
        TestUtils.Simulate.click(link);
        sinon.assert.calledOnce(fakeSetOpenState);
    }));

    it('should render menu items if routes configured', sinon.test(function () {
        let menuItemsAsReactEls = nav.renderMenuItems();
        expect(menuItemsAsReactEls.length).to.equal(2);
    }));
});
