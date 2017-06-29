import React from 'react';
import reactMixin from 'react-mixin';
import Urma from 'meteor/digi:urma-core';
import Formsy from 'formsy-react';
import Validations from '../../forms/validations';
import { hasNumbersAndLetters } from '../../forms/custom-validation-rules';
import { equalsFieldOrEmpty } from '../../forms/custom-validation-rules';

let Form = Formsy.Form;
let Input = Urma.FormSubcomponents.Input;
let alerts = Urma.alerts;

export default class ChangePassword extends React.Component {
    constructor(props){
        super(props);
        this.username = '';
        this.password = '';
        this.state = {
            submitted: false,
            valid: true
        };
        this.updatePassword = this.updatePassword.bind(this);
        this.setInputVals = this.setInputVals.bind(this);
    }

    componentWillMount(){
        //if we've already logged in and are coming back to this page
        this.props.buttonConfig(
            '/getting-started/welcome',
            '/getting-started/connect',
            this.updatePassword);

        this.props.enableNext();
    }

    getMeteorData(){
        return {
            user: Meteor.user()
        }
    }

    setInputVals(currentValues){
        this.username = currentValues.username;
        this.password = currentValues.password;
    }

    isValid(){
        this.setState({valid: true});
    }

    isInvalid(){
        this.setState({valid: false});
    }

    updatePassword(){
        let userId = this.data.user._id;
        let username = this.username;
        let password = this.password;
        let self = this;

        this.setState({submitted: true});

        if(this.state.valid) {
            Meteor.call('resetUsernamePwd', userId, username, password, function (err, result) {
                if(err){
                    alerts.error('Error' + err + 'Your changes have not been saved');
                } else {
                    self.props.transitionToNext();
                }
            });
        }

        return {
            transitionToNext: false
        }
    }

    render(){
        return (
            <div>
                <h1>Login</h1>
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR_frontPanel.jpg" alt="Front Panel"/>
                        <img src="/images/LR_backPanel.jpg" alt="Back Panel"/>
                    </div>
                    <div className="wizard-content-text">
                        <p>You may change the default username and password for improved device security.</p>
                        <p>Usernames should be 8-12 characters in length, avoiding special characters.</p>
                        <p>Passwords must be at least 6 characters in length, including at least one number and one character.</p>
                        <p>Refer to the Quick Start Guide as needed.</p>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    {this.state.submitted && !this.state.valid ?
                        <p className="error form-submitted-error bold">Form could not be submitted. Please correct highlighted errors.</p>
                        :
                        null
                    }
                    {this.data.user ?
                        <Form onValid={this.isValid.bind(this)} onInvalid={this.isInvalid.bind(this)} onChange={this.setInputVals}>
                            <Input name="username"
                                   type="text"
                                   title="Username"
                                   validations={Validations.username.userValidations}
                                   validationErrors={Validations.username.userValidationErrs}/>
                            <Input name="password"
                                   type="password"
                                   title="Password"
                                   validations={Validations.password.passwordValidations}
                                   validationErrors={Validations.password.passwordValidationErrs}/>
                            <Input name="confirmpassword"
                                   type="password"
                                   title="Confirm Password"
                                   validations="equalsFieldOrEmpty:password"
                                   validationError="Does not match password."/>
                        </Form>
                        :
                        <p>Fetching user information...</p>

                    }
                </div>
            </div>
        )
    }
}

reactMixin(ChangePassword.prototype, ReactMeteorData);