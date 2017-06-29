import Firmware from './components/firmware.jsx';

export default {
    name: 'firmware-update',
    path: 'firmware',
    component: Firmware,
    nav: {
        menuLabel: 'Update Firmware',
        image: 'fa fa-download',
        order: 3
    },
    childRoutes: []
};
