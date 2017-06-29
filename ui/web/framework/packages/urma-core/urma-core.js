import alerts from './layout/alerts.js';
import DeviceUI from './layout/components/device-ui.jsx';
import MainContent from './layout/components/main-content.jsx';
import Header from './layout/components/header.jsx';
import Navigation from './navigation/components/navigation.jsx';
import ContextualLink from './navigation/components/contextual-link.jsx';
import MenuItem from './navigation/components/menu-item.jsx';
import Authorized from './security/components/authorized.jsx';
import LoginButtons from './security/components/login-buttons.jsx';
//Helper Classes
import Login from './security/login.js';
import RouterHelper from './navigation/router-helper.js';
// Widgets
import ProgressBar from './widgets/components/progress-bar.jsx';
import Wizard from './wizard/components/wizard.jsx';
import WizardButtons from './wizard/components/wizard-buttons.jsx';
import CountdownTimer from './widgets/components/countdown-timer.jsx';
import WizardLogout from './wizard/components/wizard-logout.jsx';

// Collections
import Settings from './collections/settings.js';
import State from './collections/state.js';
import Files from './collections/files.js';
// SubsManager configuration settings
import subscriptionCacheConfig from './config.js';
//Simulators
import Simulators from './dev-utils/simulators.js';

/**
 * Core Urma object to be exported from the package. Holds references to any package scoped objects to be
 * exposed outside of the package.
 */
export default Urma = {
    alerts: alerts,
    DeviceUI: DeviceUI,
    Authorized: Authorized,
    RouterHelper: RouterHelper,
    Login: Login,
    LoginButtons: LoginButtons,
    Navigation: Navigation,
    Header: Header,
    ContextualLink: ContextualLink,
    ProgressBar: ProgressBar,
    Wizard: Wizard,
    CountdownTimer: CountdownTimer,
    Simulators: Simulators,
    // Expose global meteorhacks subscription manager directly
    SubsManager: new SubsManager(subscriptionCacheConfig),
    //this is where device uis will store their route and device type info.
    DeviceUIStore: []
};

/**
 * Core collections for URMA devices
 * @type {{Settings: Mongo.Collection, State: Mongo.Collection, Files: Mongo.Collection}}
 */
export const Collections =  {
    Settings: Settings,
    State: State,
    Files: Files
};

/**
 * Internal objects that need to be exported for package unit testing. These are considered private and should not be
 * used by the application.
 * @type {{MenuItem: MenuItem}}
 */
export const UrmaTest = {
    MenuItem: MenuItem,
    MainContent: MainContent,
    WizardButtons: WizardButtons,
    WizardLogout: WizardLogout
};
