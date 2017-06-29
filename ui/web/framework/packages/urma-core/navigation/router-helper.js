
class RouterHelper {
    constructor() {
        this._securityEnabled = true;
        this._mainMenuItems = [];
        this._defaultPath = '/';
    }

    setSecurityEnabled(enabled) {
        this._securityEnabled = (enabled ? true : false);
    }

    configureRoutes(routes, defaultPath) {
        this._defaultPath = defaultPath || '/';
        this._configureRoute(routes);
        return routes;
    }

    createMenu() {
        var orderedMenuItems = [];
        var self = this;

        this._mainMenuItems.forEach(function (item) {
            if(!item.nav.parent){
                //add all the top level items to the menu array
                orderedMenuItems.splice(item.nav.order, 0, item);
            } else {
                var parent = self._findParentMenuItem(self._mainMenuItems, item.nav.parent);
                if (parent !== undefined) {
                    parent.children.splice(item.nav.order, 0, item);
                }
            }
        });
        return orderedMenuItems;
    }

    /**
     * Configure any additional navigation properties for a route.
     * @param route The route
     * @param path An optional path for a navigation menu associated with the route. This is typically only passed in
     * for indexRoute entries. Otherwise the path property of the route is used.
     * @private
     */
    _configureRoute(route, path) {
        let menuPath = path || route.path || '#';

        //check to see if route is public and, if not, check if user is logged in before resolving
        if(route.public !== true) {
            this._onEnterCheckAuth(route);
        }

        //create menu config object and add to array that we'll use to generate menu.
        if (route.nav) {
            var menuConfigObj = {};
            menuConfigObj.nav = route.nav;
            menuConfigObj.name = route.name ? route.name : 'name' + this._mainMenuItems.length++;
            menuConfigObj.url = menuPath;
            menuConfigObj.children = [];

            this._mainMenuItems.push(menuConfigObj);
        }

        if (route.indexRoute) {
            this._configureRoute(route.indexRoute, route.path);
        }

        if (route.childRoutes) {
            route.childRoutes.forEach(function(childRoute) {
                this._configureRoute(childRoute);
            }, this);
        }
    }

    /**
     * Check if user is logged in before allowing transition to route.
     * Wraps any existing route onEnter function and calls through if authorized.
     * @param route The route being authorized
     */
    _onEnterCheckAuth(route) {
        let originalOnEnter = route.onEnter;
        let self = this;
        route.onEnter = function (nextState, replace) {
            let authorized = (self._securityEnabled ? !!Meteor.user() : true);
            if (!authorized) {
                replace(self._defaultPath);
            } else if (originalOnEnter) {
                // Call through
                originalOnEnter.apply(this, arguments);
            }
        }
    };

    /**
     * Find a menu item's parent. Recursively searches the children of the menu items.
     * @param menuItems List of potential parent menu items.
     * @param parentState The name of the parent state for the child item to add.
     * @return the parent menu item or undefined if not found.
     */
    _findParentMenuItem(menuItems, parentRoute) {
        let parent = _.find(menuItems, function(item) { return item.name === parentRoute});
        if (parent === undefined) {
            menuItems.forEach(function(menuItem) {
                parent = this._findParentMenuItem(menuItem.children);
                // returning false from forEach will cause it to stop looping, so return
                // false if we find the parent.
                return (parent === undefined);
            });
        }
        return parent;
    }

}

export default RouterHelper;