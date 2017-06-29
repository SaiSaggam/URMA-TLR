import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let MenuItem = UrmaTest.MenuItem;
let routerHelper = new Urma.RouterHelper();
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


describe('Menu Item Tests', function () {
    let routes, root, menuItems, menuEl;

    class Wrapper extends React.Component {
        getChildContext() {
            return {
                basePath: '/',
                devicePath: 'TLR'
            };
        }
        render() {
            return (
                <MenuItem
                    level={1}
                    openState={fakeInitState}
                    menuItem={fakeMenuObj}
                    setOpenState={fakeFunc}
                    openStateClass={fakeFunc}/>
            )
        }
    }
    Wrapper.childContextTypes = {
        basePath: React.PropTypes.string.isRequired,
        devicePath: React.PropTypes.string.isRequired
    };

    routes = routerHelper.configureRoutes({
        path: '/',
        component: Wrapper,
        public: true
    }, '/');

    root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
    menuItems = TestUtils.scryRenderedComponentsWithType(root, MenuItem);

    //get the top of level menu item
    menuEl = menuItems[0];


    it('should contain create three level of menu items', function () {

        let topLevelMenuEl = TestUtils.findRenderedDOMComponentWithClass(menuEl, 'config-nav-main-listitem-first');
        let secondLevelMenuEl = TestUtils.findRenderedDOMComponentWithClass(menuEl, 'config-nav-main-listitem-second');
        let thirdLevelMenuEl = TestUtils.findRenderedDOMComponentWithClass(menuEl, 'config-nav-main-listitem-third');

        expect(topLevelMenuEl).to.not.be.undefined;
        expect(secondLevelMenuEl).to.not.be.undefined;
        expect(thirdLevelMenuEl).to.not.be.undefined;

        let linkLevels = TestUtils.scryRenderedDOMComponentsWithClass(menuEl, 'main-menu-label');

        expect(linkLevels[0].textContent).to.equal('Top');
        expect(linkLevels[1].textContent).to.equal('Second');
        expect(linkLevels[2].textContent).to.equal('Third');
    });

    it('should update open state and toggle on click', sinon.test(function () {

        let links = TestUtils.findAllInRenderedTree(menuEl, function (elem) {
            return TestUtils.isCompositeComponentWithType(elem, Link);
        });
        let spyHandleClick = this.spy(menuEl, 'handleClick');
        //TestUtils.Simulate.click(links[0]);
        //sinon.assert.calledOnce(spyHandleClick);
    }));

});
