import React from 'react';
import Urma from 'meteor/digi:urma-core';

//
//export class Login extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/welcome', '/getting-started/connect', function () {
//            //console.log(LoginButtons);
//            //LoginButtons.login();
//        });
//        this.props.enableNext();
//    }
//    render(){
//        return (
//                <div>
//                 <h1>Login</h1>
//                <LoginButtons></LoginButtons>
//            </div>
//        )
//    }
//};

// export class Connect extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/login', '/getting-started/cellular');
//        this.props.enableNext();
//    }
//    render(){
//        return (
//            <div>
//                <h1>Choose to connect with ethernet or cellular</h1>
//            </div>
//        )
//    }
// };

// export class Cellular extends React.Component {
//     componentWillMount(){
//         this.props.buttonConfig('/getting-started/connect', '/getting-started/firmware-update');
//         this.props.enableNext();
//     }
//     render(){
//         return (
//             <div>
//                 <h1>You chose cellular, give us more info</h1>
//             </div>
//         )
//     }
// };

export class Ethernet extends React.Component {
    componentWillMount(){
        this.props.buttonConfig('/getting-started/connect', '/getting-started/firmware-update');
        this.props.enableNext(); 
    }
    render(){
        return (
            <div>
                <h1>You chose ethernet, give us more info</h1>
            </div>
        )
    }
};

// export class CellularResults extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/cellular', '/getting-started/firmware-update');
//        this.props.enableNext();
//    }
//    render(){
//        return (
//            <div>
//                <h1>Here are the results of your connection attempt</h1>
//            </div>
//        )
//    }
// };

//export class FWUpdate extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/cellular-results', '/getting-started/remote-manager-connect');
//        this.props.enableNext();
//    }
//    render(){
//        return (
//            <div>
//                <h1>Update Firmware</h1>
//            </div>
//        )
//    }
//};

//export class DRMConnect extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/firmware-update', '/getting-started/about-dashboard');
//        this.props.enableNext();
//    }
//    render(){
//        return (
//            <div>
//                <h1>Connect to Remote Manager</h1>
//            </div>
//        )
//    }
//};
//
//export class AboutDashboard extends React.Component {
//    componentWillMount(){
//        this.props.buttonConfig('/getting-started/remote-manager-connect', '/');
//        this.props.enableNext();
//    }
//    render(){
//        return (
//            <div>
//                <h1>About the dashboard</h1>
//            </div>
//        )
//    }
//};

//;
export class AboutDashboard extends React.Component {
   componentWillMount(){
       this.props.buttonConfig('/getting-started/remote-manager-connect', '/');
       this.props.enableNext();
   }
   render(){
       return (
           <div>
               <h1>About the dashboard</h1>
           </div>
       )
   }
};

export default getStarted = {
    //Welcome: Welcome,
    //Login: Login,
    //Connect: Connect,
    //Cellular: Cellular,
    //CellularResults: CellularResults,
    Ethernet: Ethernet,
    //FWUpdate: FWUpdate,
    //DRMConnect: DRMConnect,
    AboutDashboard: AboutDashboard

};
