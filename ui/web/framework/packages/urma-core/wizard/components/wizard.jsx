import React from 'react';
import reactMixin from 'react-mixin';
import { Link } from 'react-router';
import Header from '../../layout/components/header.jsx';
import MainContent from '../../layout/components/main-content.jsx';
import WizardButtons from './wizard-buttons.jsx';
import WizardLogout from './wizard-logout.jsx';

export default class Wizard extends React.Component {
    constructor(props){
        super(props);
        this.state = {
            back: '/',
            next: '/',
            onNext: null,
            nextEnabled: true
        };
    }

    getMeteorData() {
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;
        let title = '';
        let search = {_groupName: 'system'};
        let subscription = SubsManager.subscribe('state', search);
        if (subscription.ready()) {
            //TODO: pass title (or device family and model) in as properties.
            let system = State.findOne(search) || {};
            title = 'Digi Transport';
            if (system.model) {
                title += ' | '+system.model;
            }
        }

        return {
            title: title,
            user: Meteor.user()
        };
    }


    componentWillMount() {
         //Ensure if we are not logged in when mounting we send them to login page/
        if (!this.data.user
            && this.props.location
            && !this.props.children.props.route.public) {
                this.context.router.push(`/getting-started`);
        }
    }

    componentDidUpdate(prevProps, prevState) {
        // When data changes ensure if user logged out we navigate to login page.
        if (!this.data.user
            && this.props.location
            && !this.props.children.props.route.public) {
                this.context.router.push(`/getting-started`);
        }
    }

    buttonConfig(back, next, onNextCb) {
        this.setState({
            back: back,
            next: next,
            onNext: onNextCb
        });
    }

    disableNext(){
        this.setState({nextEnabled: false})
    }

    enableNext(){
        this.setState({nextEnabled: true})
    }

    transitionToNext(){
        this.context.router.push(this.state.next);
    }

    render(){
        return (
            <div>
                <div className='header-wrapper'>
                    <header>
                        <h2><img src='/images/logo.png' alt='Digi International'/><div>{this.data.title}</div></h2>
                        {this.data.user ?
                            <WizardLogout/>
                            :
                            null
                        }
                    </header>
                </div>
                <div className="main-container">
                    <div className="config-main">
                        {this.props.children && React.cloneElement(this.props.children, {
                            buttonConfig: this.buttonConfig.bind(this),
                            disableNext: this.disableNext.bind(this),
                            enableNext: this.enableNext.bind(this),
                            transitionToNext: this.transitionToNext.bind(this),
                            loggedIn: this.data.user? true : false
                        })}
                    </div>
                </div>
                <WizardButtons onNext={this.state.onNext} history={this.props.history} transitionToNext={this.transitionToNext.bind(this)} back={this.state.back} next={this.state.next} nextEnabled={this.state.nextEnabled}></WizardButtons>
            </div>
        )
    }
}

Wizard.contextTypes = {
    router: React.PropTypes.object.isRequired,
    device: React.PropTypes.object.isRequired
};

reactMixin(Wizard.prototype, ReactMeteorData);