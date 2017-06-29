import React from 'react';
import Urma from 'meteor/digi:urma-core';

//buttonConfig passed down from wizard. Takes three arguments--route for back button,
// route for the next button, function to be called when next button is clicked.

export default class Welcome extends React.Component {

    componentWillMount(){
        this.props.buttonConfig(null, '/getting-started/login');
        this.props.enableNext();
    }

    render(){
        return (
            <div className="welcome-getting-started">
                <h1>Welcome to the Getting Started Wizard</h1>
                <div className="wizard-content-area-left">
                    <p>Getting Started Wizard will assist you with :</p>
                    <ol className="default">
                        <li>Connecting your device to a Wide Area Network</li>
                        <li>Updating the device's firmware</li>
                        <li>Registering the device with Digi Remote Manager</li>
                    </ol>
                    <p>Locate the Quick Start Guide, included in the package and referenced below.</p>
                    <p>Click on START DEVICE SETUP, on the lower-right.</p>
                    <div className="wizard-links margin-top">
                        <a href="http://www.digi.com/resources/documentation/digidocs/pdfs/90001460-88.pdf" target="_blank"><img border="0" alt="quick start guide" src="/images/quick_start_guide.png" width="50" height="50"/>Quick Start Guide</a>
                        <a href="http://www.digi.com/resources/documentation/digidocs/90001461/default.htm" target="_blank"><img border="0" alt="online user manual" src="/images/online_user_manual.png" width="50" height="50"/>Online User Manual</a>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    <img src="/images/LR54-AW401_Ant_5x5.jpg" alt="LR54"/>
                    {/*<a href="#" className="next">SKIP TO DASHBOARD <i className="fa fa-caret-right"></i></a>*/}
                </div>
            </div>
        )
    }
};