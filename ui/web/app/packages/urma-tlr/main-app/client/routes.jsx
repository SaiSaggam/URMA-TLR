import MainLayout from './components/main-layout.jsx';
import Home from './components/home.jsx';
import Urma from 'meteor/digi:urma-core';

import managementRoutes from '../../management/client/index.jsx';
import fileSysRoutes from '../../file-system/client/index.jsx';
import firmwareRoutes from '../../firmware/index.jsx';

const routes = {
    path: '/',
    component: MainLayout,
    indexRoute: {
        name: 'home',
        component: Home,
        nav: {
            menuLabel: 'Home',
            image: 'fa fa-home',
            order: 0
        }
    },
    childRoutes: [
        {
            path: 'getting-started',
            nav: {
                menuLabel: 'Getting Started',
                image: 'fa fa-magic',
                order: 4
            },
            indexRedirect: {
                to: '/getting-started'
            },
            children: []
        }
    ]
};

export default routes;
