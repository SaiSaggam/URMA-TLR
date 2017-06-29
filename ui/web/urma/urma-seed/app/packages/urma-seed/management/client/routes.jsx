import EthernetList from './components/interfaces/ethernet-list.jsx';
import Ethernet from './components/interfaces/ethernet.jsx';

export default {
    name: 'management',
    nav: {
        menuLabel: 'Management',
        image: 'fa fa-cogs',
        order: 1
    },
    childRoutes: [
        {
            name: 'interfaces',
            nav: {
                menuLabel: 'Interfaces',
                parent: 'management',
                order: 0
            },
            childRoutes: [
                {
                    name: 'ethernet',
                    path: 'management/ethernet',
                    component: EthernetList,
                    nav: {
                        menuLabel: 'Ethernet',
                        parent: 'interfaces',
                        order: 0
                    }
                },
                {
                    name: 'ethernet-edit',
                    path: 'management/ethernet/:_groupIndex',
                    component: Ethernet
                }
            ]
        }
    ]
};
