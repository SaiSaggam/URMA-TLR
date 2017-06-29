import MainLayout from './components/main-layout.jsx';
import Home from './components/home.jsx';

import managementRoutes from '../../management/client/index.jsx';
import fileSysRoutes from '../../file-system/client/index.jsx';

let routerHelper = Urma.routerHelper;

routerHelper.setSecurityEnabled(true);

export default routerHelper.configureRoutes({
    path: '/',
    component: MainLayout,
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
        managementRoutes,
        fileSysRoutes
    ]
}, '/');
