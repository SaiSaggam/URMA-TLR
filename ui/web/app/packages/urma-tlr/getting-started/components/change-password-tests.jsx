import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import {Router, browserHistory} from 'react-router';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';
import ChangePassword from './change-password.jsx';
import Formsy from 'formsy-react';

let Form = Formsy.Form;
let Input = Urma.FormSubcomponents.Input;
let Wizard = Urma.Wizard;

let changePassword,
    root,
    form,
    inputs,
    wizard,
    sandbox,
    nextButton,
    usernameInput,
    passwordInput,
    confirmPasswordInput,
    stubUser;

describe('Change Password Tests', ()=> {

    let stubProps = {
        buttonConfig: sinon.stub(),
        enableNext: sinon.stub(),
        disableNext: sinon.stub(),
        transitionToNext: sinon.stub()
    };

    beforeEach(()=> {
        sandbox = sinon.sandbox.create();
        stubUser = sandbox.stub(Meteor, "user", ()=> {return {username: 'useruser'}});
        changePassword = TestUtils.renderIntoDocument(
            <ChangePassword buttonConfig={stubProps.buttonConfig} enableNext={stubProps.enableNext}/>
        );
        form = TestUtils.findRenderedComponentWithType(changePassword, Form);
        inputs = TestUtils.scryRenderedComponentsWithType(form, Input);
        usernameInput = inputs[0];
        passwordInput = inputs[1];
        confirmPasswordInput = inputs[2];
    });

    afterEach(()=> {
        sandbox.restore();
    });

    it('should render change password form', function () {
        expect(form).to.not.be.undefined;
        sinon.assert.calledOnce(stubProps.enableNext);
    });

    it('should show username error on blur', function () {
        let username = TestUtils.findRenderedDOMComponentWithTag(usernameInput, 'input');

        TestUtils.Simulate.change(username, {target: {value: 'short'}});
        TestUtils.Simulate.blur(username);

        let error = TestUtils.findRenderedDOMComponentWithClass(usernameInput, 'error');
        
        expect(error.textContent).to.equal('Username must be at least 8 characters in length.');

        TestUtils.Simulate.change(username, {target: {value: 'muchtoolooooooooong'}});
        TestUtils.Simulate.blur(username);

        expect(error.textContent).to.equal('Username cannot be more than 12 characters in length.');

        TestUtils.Simulate.change(username, {target: {value: '!!!special'}});
        TestUtils.Simulate.blur(username);

        expect(error.textContent).to.equal('Username may not contain special characters.');

    });

    it('should show password error on blur', function () {
        let password = TestUtils.findRenderedDOMComponentWithTag(passwordInput, 'input');

        TestUtils.Simulate.change(password, {target: {value: 'short'}});
        TestUtils.Simulate.blur(password);

        let error = TestUtils.findRenderedDOMComponentWithClass(passwordInput, 'error');

        expect(error.textContent).to.equal('Username must be at least 6 characters in length.');

        TestUtils.Simulate.change(password, {target: {value: 'nonumbers'}});
        TestUtils.Simulate.blur(password);

        expect(error.textContent).to.equal('Password must contain at least one number and one letter.');

    });

    it('should show error if password and confirm password fields do not match', function () {
        let password = TestUtils.findRenderedDOMComponentWithTag(passwordInput, 'input');
        let confirmPassword = TestUtils.findRenderedDOMComponentWithTag(confirmPasswordInput, 'input');

        TestUtils.Simulate.change(password, {target: {value: 'nomatch1'}});
        TestUtils.Simulate.change(confirmPassword, {target: {value: 'nomatch2'}});
        TestUtils.Simulate.blur(confirmPassword);

        let error = TestUtils.findRenderedDOMComponentWithClass(confirmPasswordInput, 'error');
        expect(error.textContent).to.equal('Does not match password.');
    });
});

describe('Change Password Tests (Wizard)', ()=> {

    beforeEach(()=> {
        sandbox = sinon.sandbox.create();
        stubUser = sandbox.stub(Meteor, "user", ()=> {return {username: 'useruser'}});
        let stubSubscriptionStub = { ready: sandbox.stub().returns(true)};
        let stubSubsManager = { subscribe: sandbox.stub().returns(stubSubscriptionStub) };
        let stubState = {
            findOne: sandbox.stub()
        };

        let deviceCtx = {
            SubsManager: stubSubsManager,
            State: stubState
        };

        class WizardParent extends React.Component {
            getChildContext() {
                return {
                    device: deviceCtx
                };
            }

            render(){
                return (
                    <div>{this.props.children}</div>
                )
            }
        }

        WizardParent.childContextTypes = {
            device: React.PropTypes.object.isRequired
        };

        let routes = {
            component: WizardParent,
            public: true,
            childRoutes: [{
                name: 'wizard',
                path: '/',
                component: Wizard,
                indexRoute: {
                    name: 'change-password',
                    component: ChangePassword,
                    public: true
                }
            }]
        };

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory} />);
        wizard = TestUtils.findRenderedComponentWithType(root, Wizard);
        changePassword = TestUtils.findRenderedComponentWithType(wizard, ChangePassword);
        nextButton = TestUtils.findRenderedDOMComponentWithClass(wizard, 'next');
        inputs = TestUtils.scryRenderedComponentsWithType(changePassword, Input);
        usernameInput = inputs[0];
        passwordInput = inputs[1];
        confirmPasswordInput = inputs[2];
    });

    afterEach(()=> {
        sandbox.restore();
    });

    it('should show username error on submit', function () {
        let username = TestUtils.findRenderedDOMComponentWithTag(usernameInput, 'input');

        TestUtils.Simulate.change(username, {target: {value: 'short'}});
        TestUtils.Simulate.blur(username);
        TestUtils.Simulate.click(nextButton);

        expect(changePassword.state.valid).to.equal(false);
        expect(changePassword.state.submitted).to.equal(true);

        let error = TestUtils.findRenderedDOMComponentWithClass(usernameInput, 'error');
        let onNextError = TestUtils.findRenderedDOMComponentWithClass(changePassword, 'form-submitted-error');

        expect(error.textContent).to.equal('Username must be at least 8 characters in length.');
        expect(onNextError.textContent).to.equal('Form could not be submitted. Please correct highlighted errors.');
    });

    it('should call resetUsernamePwd on Next only if form valid', function () {
        let stubMeteorCall = sandbox.stub(Meteor, 'call');
        let password = TestUtils.findRenderedDOMComponentWithTag(passwordInput, 'input');
        let confirmPassword = TestUtils.findRenderedDOMComponentWithTag(confirmPasswordInput, 'input');

        TestUtils.Simulate.change(password, {target: {value: 'nonumberz'}});
        TestUtils.Simulate.blur(password);
        TestUtils.Simulate.click(nextButton);

        sinon.assert.notCalled(stubMeteorCall);

        TestUtils.Simulate.change(password, {target: {value: 'numberz1'}});
        TestUtils.Simulate.change(confirmPassword, {target: {value: 'numberz1'}});
        TestUtils.Simulate.click(nextButton);

        sinon.assert.calledOnce(stubMeteorCall);
    });

});