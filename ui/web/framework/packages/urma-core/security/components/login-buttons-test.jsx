import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let LoginButtons = Urma.LoginButtons;
let Login = Urma.Login;

describe('LoginButtons Component Tests', function() {

    it('should render login form when not logged in', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(
            <LoginButtons/>
        );

        // Not logged in. Should render a form with username, password fields and login button.
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let username = inputs.find((input)=> {return input.placeholder == 'username'});
        let password = inputs.find((input)=> {return input.placeholder == 'password'});
        let loginBtn = TestUtils.findRenderedDOMComponentWithTag(root, 'button');

        expect(form.getAttribute('class')).to.equal('login-buttons');
        expect(username.type).to.equal('text')
        expect(password.type).to.equal('password');
        expect(loginBtn.type).to.equal('submit');
        expect(loginBtn.textContent).to.equal('Login');
    }));

    it('should render logout button when logged in', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});
        let root = TestUtils.renderIntoDocument(
            <LoginButtons/>
        );

        sinon.assert.calledOnce(stubUser);

        // Logged in. Should render a form with a logout button.
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let logoutBtn = TestUtils.findRenderedDOMComponentWithTag(root, 'button');

        expect(form.getAttribute('class')).to.equal('login-buttons');
        expect(logoutBtn.textContent).to.equal('Logout');
    }));

    it('should call login when submit login form', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(
            <LoginButtons/>
        );
        let loginService = root.loginService();
        let stubDeviceLogin = this.stub(loginService, 'login');

        // Simulate logging in
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let username = inputs.find((input)=> {return input.placeholder == 'username'});
        let password = inputs.find((input)=> {return input.placeholder == 'password'});

        // Simulate setting username and password, and submit login form
        TestUtils.Simulate.change(username, { target: { value: 'user' } });
        TestUtils.Simulate.change(password, { target: { value: 'mypassword' } });
        TestUtils.Simulate.submit(form);

        sinon.assert.calledOnce(stubDeviceLogin);
    }));

    it('should call logout when click logout', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});
        let root = TestUtils.renderIntoDocument(
            <LoginButtons/>
        );
        let loginService = root.loginService();
        let stubDeviceLogout = this.stub(loginService, 'logout');

        sinon.assert.calledOnce(stubUser);

        // Logged in. Should render a form with a logout button.
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let logoutBtn = TestUtils.findRenderedDOMComponentWithTag(root, 'button');
        TestUtils.Simulate.click(logoutBtn);

        sinon.assert.calledOnce(stubDeviceLogout);
    }));

    it('should handle error when login fails', sinon.test(function() {
        let err = new Meteor.Error("login failure", "Invalid user.");
        let root = TestUtils.renderIntoDocument(
            <LoginButtons/>
        );
        let loginService = root.loginService();
        let stubDeviceLogin = this.stub(loginService, 'login', function(username, password, cb) {
            // Call login callback with error
            cb(err);
        });

        // Just listen for the showError function on the loginbuttons component to be called when login fails
        let stubShowError = this.stub(root, "showError", function(){}   );

        // Simulate logging in (with failure)
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        TestUtils.Simulate.submit(form);

        sinon.assert.calledOnce(stubDeviceLogin);
        sinon.assert.calledWith(stubShowError, 'login', err);
    }));

    it('should call custom login', sinon.test(function() {
        let stubDeviceLoginService = { login: sinon.stub() };
        let root = TestUtils.renderIntoDocument(
            <LoginButtons loginService={stubDeviceLoginService}/>
        );

        // Simulate logging in
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let username = inputs.find((input)=> {return input.placeholder == 'username'});
        let password = inputs.find((input)=> {return input.placeholder == 'password'});

        // Simulate setting username and password, and submit login form
        TestUtils.Simulate.change(username, { target: { value: 'user' } });
        TestUtils.Simulate.change(password, { target: { value: 'mypassword' } });
        TestUtils.Simulate.submit(form);

        sinon.assert.calledOnce(stubDeviceLoginService.login);
        sinon.assert.calledWith(stubDeviceLoginService.login, 'user', 'mypassword');
    }));

    it('should call custom logout', sinon.test(function() {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});
        let stubMeteorLogout = this.stub(Meteor, 'logout');
        let stubDeviceLoginService = { logout: sinon.stub() };

        let root = TestUtils.renderIntoDocument(
            <LoginButtons loginService={stubDeviceLoginService}/>
        );

        sinon.assert.calledOnce(stubUser);

        // Logged in. Should render a form with a logout button.
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let logoutBtn = TestUtils.findRenderedDOMComponentWithTag(root, 'button');
        TestUtils.Simulate.click(logoutBtn);

        sinon.assert.notCalled(stubMeteorLogout);
        sinon.assert.calledOnce(stubDeviceLoginService.logout);
    }));

});
