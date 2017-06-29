import reactMixin from 'react-mixin';
import React from 'react';
import LinkedStateMixin from 'react-addons-linked-state-mixin'
import Urma from 'meteor/digi:urma-core';

export default class Login extends Urma.Login {
    constructor(props) {
        super(props);
        this.state = {username: '', password: ''};
        this.logInOnNext = this.logInOnNext.bind(this);
    }

    componentWillMount(){
        //if we've already logged in and are coming back to this page
        this.props.buttonConfig(
            '/getting-started/welcome',
            '/getting-started/change-password',
            this.logInOnNext);

        this.props.enableNext();
    }

    componentWillReceiveProps(){
        if (this.props.loggedIn) {
            this.props.transitionToNext();
        }
    }

    logInOnNext(e){
        let self = this;

        if(!this.props.loggedIn) {
            this.login(e, function () {
                self.props.transitionToNext();
            });
        } else {
            self.props.transitionToNext();
        }

        return {
            transitionToNext: false
        }

    }

    checkLoggedIn(){
        return (
            <div className="user-logged-in">
                <h1>You are already logged in.</h1>
            </div>
        )
    }

    renderLogin(){
        return (
            <div>
                <h1>Login</h1>
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR_frontPanel.jpg" alt="Front Panel"/>
                        <img src="/images/LR_backPanel.jpg" alt="Back Panel"/>
                    </div>
                    <div className="wizard-content-text">
                        <p>Enter the username and password for this device.</p>
                        <p>Refer to the Quick Start Guide in the package to learn default values for each field.</p>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    <form>
                        <div>
                            <div className="box">
                                <label>Username</label>
                                <input type="text" className="form-control" valueLink={this.linkState('username')}
                                       placeholder="username"/>
                            </div>
                            <div className="box">
                                <label>Password</label>
                                <input type="password" className="form-control" valueLink={this.linkState('password')}
                                       placeholder="password"/>
                            </div>
                            <p>Click NEXT to login to device.</p>
                        </div>
                    </form>
                </div>
            </div>
        )
    }

    render(){
        return (
            <div>
                {this.props.loggedIn ?
                    this.checkLoggedIn() :
                    this.renderLogin() }
            </div>

        )
    }
};

reactMixin(Login.prototype, LinkedStateMixin);
