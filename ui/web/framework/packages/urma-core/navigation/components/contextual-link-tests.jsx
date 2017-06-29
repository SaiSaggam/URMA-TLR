import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';

let ContextualLink = Urma.ContextualLink;
let fakeFunc = function (){};
let fakeInitState = true;

let fakeMenuObj = {
    name: 'top',
    url: '/top',
    isTransitionable: true,
    children: [{
        name: 'second',
        url: '/second',
        isTransitionable: true,
        children: [{
            name: 'third',
            url: '/third',
            isTransitionable: true,
            children: [],
            nav: {
                menuLabel: 'Third'
            }
        }],
        nav: {
            menuLabel: 'Second'
        }
    }],
    nav: {
        menuLabel: 'Top'
    }

};


describe('Contextual Link Tests', function () {
    let root, cntxtLink;
    let devicePath = 'TLR';

    it('should render link with no prefix on path', function () {
        //simulate the device context
        class Wrapper extends React.Component {
            getChildContext() {
                return {
                    basePath: '/',
                    devicePath: '/'
                };
            }
            render() {
                return (
                    <ContextualLink to={'fake'} className='test'>Test</ContextualLink>
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
            public: true
        };

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        cntxtLink = TestUtils.findRenderedComponentWithType(root, ContextualLink);

        let link = TestUtils.findRenderedDOMComponentWithClass(cntxtLink, 'test');
        expect(link.getAttribute('href')).to.equal('/fake');

    });

    it('should render link with prefixed path', function () {
        class Wrapper extends React.Component {
            getChildContext() {
                return {
                    basePath: '/devices/123/TLR/',
                    devicePath: devicePath
                };
            }
            render() {
                return (
                    <ContextualLink to={'fake'} className='test'>Test</ContextualLink>
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
            public: true
        };

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        cntxtLink = TestUtils.findRenderedComponentWithType(root, ContextualLink);

        let link = TestUtils.findRenderedDOMComponentWithClass(cntxtLink, 'test');
        expect(link.getAttribute('href')).to.equal('/devices/123/TLR/fake');

    });

    it('should render link with base path', function () {
        class Wrapper extends React.Component {
            getChildContext() {
                return {
                    basePath: '/devices/123/TLR/',
                    devicePath: devicePath
                };
            }
            render() {
                return (
                    <ContextualLink to={'TLR'} className='test'>Test</ContextualLink>
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
            public: true
        };

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        cntxtLink = TestUtils.findRenderedComponentWithType(root, ContextualLink);

        let link = TestUtils.findRenderedDOMComponentWithClass(cntxtLink, 'test');
        expect(link.getAttribute('href')).to.equal('/devices/123/TLR/');
    });

});
