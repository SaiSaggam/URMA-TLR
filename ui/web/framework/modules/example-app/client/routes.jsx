import ExampleApp from './example-app.jsx';
import Home from './components/home.jsx';
import UsageExample from './components/usage-example.jsx';
import Security from './components/security.jsx';
import Ethernet from './components/ethernet.jsx';
import Urma from 'meteor/digi:urma-core';

let routerHelper = new Urma.RouterHelper();

routerHelper.setSecurityEnabled(true);

export default routerHelper.configureRoutes({
  path: '/',
  component: ExampleApp,
  public: true,
  indexRoute: {
    name: 'home',
    component: Home,
    public: true,
    nav: {
      menuLabel: 'Home',
      image: 'fa fa-home',
      order: 0
    }
  },
  childRoutes: [
    {
      name: 'usage-examples',
      path: '/usage-examples',
      component: UsageExample,
      nav: {
        menuLabel: 'Usage Examples',
        image: 'fa fa-dashboard',
        order: 1
      }
    },
    {
      name: 'security',
      path: '/security',
      component: Security,
      nav: {
        menuLabel: 'Security',
        image: 'fa fa-lock',
        order: 2
      }
    },
    {
      name: 'testing2',
      path: '/testing2',
      component: UsageExample,
      nav: {
        menuLabel: 'Testing2',
        parent: 'usage-examples',
        order: 1
      }
    },
    {
      name: 'interfaces',
      nav: {
        menuLabel: 'Interfaces',
        parent: 'usage-examples',
        order: 0
      }
    },
    {
      name: 'ethernet',
      path: '/interfaces/ethernet',
      component: Ethernet,
      nav: {
        menuLabel: 'Ethernet',
        parent: 'interfaces',
        order: 0
      }
    }
  ]
}, '/');
