import reactMixin from 'react-mixin';
import React from 'react';
import LinkedStateMixin from 'react-addons-linked-state-mixin'
import Login from '../../security/login.js';

/**
 * React component containing a form with login and logout buttons.
 * @type {LoginButtons}
 */


export default class LoginButtons extends Login {

    constructor(props) {
        super(props);
        this.state = {username: '', password: ''};
    }

    getMeteorData() {
        return {
            user: Meteor.user()
        };
    }

    renderLogin() {
        return (
            <form className="login-buttons" onSubmit={this.login}>
                <div>
                    <div className="box">
                        <input type="text" className="form-control" valueLink={this.linkState('username')}
                               placeholder="username"/>
                    </div>
                    <div className="box">
                        <input type="password" className="form-control" valueLink={this.linkState('password')}
                               placeholder="password"/>
                    </div>
                    <div className="box">
                        <button type="submit">Login</button>
                    </div>
                </div>
            </form>
        )
    }

    renderLogout() {
        return (
            <form className="login-buttons">
                <div className="box">
                    <button type="button" onClick={this.logout}>Logout</button>
                </div>
            </form>
        )
    }

    render() {
        return (
            <div className="login">
                { this.data.user ? this.renderLogout() : this.renderLogin() }
            </div>
        )
    }
};

LoginButtons.propTypes = {
    loginService: React.PropTypes.object
};

reactMixin(LoginButtons.prototype, LinkedStateMixin);
reactMixin(LoginButtons.prototype, ReactMeteorData);