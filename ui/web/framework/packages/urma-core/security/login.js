import React from 'react';
import { Link } from 'react-router';
import { DeviceLoginService } from './device-login.js';

/**
 * React component that can be extended by any component that needs authentication functionality.
 * @type LoginButtons
 */


export default class Login extends React.Component {
    constructor(props) {
        super(props);
        this.loginService = this.loginService.bind(this);
        this.login = this.login.bind(this);
        this.logout = this.logout.bind(this);
    }

    resetUserPwd(passwordOnly) {
        if (passwordOnly) {
            this.setState({password: ''});
        } else {
            this.setState({username: '', password: ''});
        }
    }

    showError(errorType, err) {
        console.log('ERROR - '+errorType+': ', err);
        err = err || {};
        var reason = err.reason || 'unknown error';
        alerts.error('Failed '+errorType+' - ' + reason, 'Login Error');
    }

    loginService() {
        return this.props.loginService ? this.props.loginService : DeviceLoginService;
    }

    login(e, successFunc) {
        if(typeof e.preventDefault === 'function'){
            e.preventDefault(); // Prevent form submission.
        }

        this.loginService().login(this.state.username, this.state.password, (err) => {
            if (err) {
                this.showError('login', err);
                this.resetUserPwd(true);
            } else {
                if(typeof successFunc === 'function') {
                    successFunc();
                }
                alerts.clear(); // Clear any outstanding alerts
            }
        });
    }

    logout(e, successFunc) {
        if(typeof e.preventDefault === 'function'){
            e.preventDefault(); // Prevent form submission.
        }

        this.loginService().logout((err) => {
            if (err) {
                showError('logout', err);
            } else {
                if(typeof successFunc === 'function') {
                    successFunc();
                }
                alerts.clear();
                this.resetUserPwd();
            }
        });
    }
}