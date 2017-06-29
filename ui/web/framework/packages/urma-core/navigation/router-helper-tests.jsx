import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';

let RouterHelper = new Urma.RouterHelper();

describe('Router Helper Tests', function() {
    let routes, menu, childRoutes;

    beforeEach(function () {
        //generate routes for main menu
        routes = RouterHelper.configureRoutes({
            path: '/',
            indexRoute: {
                name: 'home',
                public: true,
                nav: {
                    menuLabel: 'Home',
                    image: 'fa fa-home',
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
                        image: 'fa fa-lock',
                        order: 2
                    }
                },
                {
                    name: 'interfaces',
                    nav: {
                        menuLabel: 'Interfaces',
                        order: 1
                    }
                },
                {
                    name: 'ethernet',
                    path: '/interfaces/ethernet',
                    nav: {
                        menuLabel: 'Ethernet',
                        parent: 'interfaces',
                        order: 1
                    }
                },
                {
                    name: 'cellular',
                    path: '/interfaces/cellular',
                    nav: {
                        menuLabel: 'Cellular',
                        parent: 'interfaces',
                        order: 0
                    }
                }
            ]
        }, '/');

        menu = RouterHelper.createMenu();
        childRoutes = routes.childRoutes;
    });

    afterEach(function () {
        //clear routes by resetting mainMenuItems array
        RouterHelper._mainMenuItems = [];
    });

    it('should create the routes', function() {
        let childRoutes = routes.childRoutes;
        expect(childRoutes.length).to.equal(4);
    });

    it('should add routes with nav object to list of menu items', function () {
        let menuItems = RouterHelper._mainMenuItems;
        expect(menuItems.length).to.equal(5);
    });

    it('should build and order submenus', function () {
        expect(menu.length).to.equal(3);
        expect(menu[1].children.length).to.equal(2);
        expect(menu[1].children[0].name).to.equal('cellular');
    });

    it('should set path to # if path is present', function () {
        expect(menu[1].url).to.equal('#');
        expect(menu[2].url).to.equal('/security');
    });

    it('should not allow logged out user to access private route', sinon.test(function () {
        let fakeReplaceState = {
            replaceState: function () {}
        };
        let spyReplaceState = this.spy(fakeReplaceState, 'replaceState');

        RouterHelper._securityEnabled = true;

        childRoutes[1].onEnter({}, fakeReplaceState.replaceState);
        sinon.assert.calledOnce(spyReplaceState);
    }));

    it('should allow access to private route if security not enabled', sinon.test(function () {
        let fakeReplaceState = {
            replaceState: function () {}
        };
        let spyReplaceState = this.spy(fakeReplaceState, 'replaceState');

        RouterHelper._securityEnabled = false;

        childRoutes[1].onEnter({}, fakeReplaceState.replaceState);
        sinon.assert.notCalled(spyReplaceState);
    }));

    it('should not restrict routes based on public property', sinon.test(function () {
        let spyCheckAuth = this.spy(RouterHelper, '_onEnterCheckAuth');

        RouterHelper._configureRoute({
            name: 'security',
            path: '/security',
            public: true
        }, '/security');

        sinon.assert.notCalled(spyCheckAuth);

        RouterHelper._configureRoute({
            name: 'security',
            path: '/security'
        }, '/security');

        sinon.assert.calledOnce(spyCheckAuth);
    }));
});