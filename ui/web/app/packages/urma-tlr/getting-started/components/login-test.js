import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';
import Login from './login.jsx';

let fakeFunc = function () {};

describe('Login tests', function () {

    it('should display username and password fields', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(
            <Login buttonConfig={fakeFunc} loggedIn={false} enableNext={fakeFunc}/>
        );

        // Not logged in. Should render a form with username and password fields.
        let form = TestUtils.findRenderedDOMComponentWithTag(root, 'form');
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let username = inputs.find((input)=> {return input.placeholder == 'username'});
        let password = inputs.find((input)=> {return input.placeholder == 'password'});

        expect(username.type).to.equal('text');
        expect(password.type).to.equal('password');
    }));

    it('should display notice that user is logged in if user already logged ', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(
            <Login buttonConfig={fakeFunc} loggedIn={true} enableNext={fakeFunc}/>
        );

        let container = TestUtils.findRenderedDOMComponentWithClass(root, 'user-logged-in');
        expect(container.textContent).to.equal('You are already logged in.');
    }));

    it('should call transitionToNext when next clicked if user already logged in', sinon.test(function () {
        let spyTransition = this.spy();
        let root = TestUtils.renderIntoDocument(
            <Login buttonConfig={fakeFunc} loggedIn={true} enableNext={fakeFunc} transitionToNext={spyTransition}/>
        );

        root.logInOnNext({});
        sinon.assert.calledOnce(spyTransition);

    }));

    it('should attempt login on next', sinon.test(function () {
        let spyTransition = this.spy();
        let root = TestUtils.renderIntoDocument(
            <Login buttonConfig={fakeFunc} loggedIn={false} enableNext={fakeFunc} transitionToNext={spyTransition}/>
        );

        let spyLoginService = this.stub(root, 'loginService', function () {
            return {
                login: fakeFunc,
                logout: fakeFunc
            }
        });

        root.logInOnNext({
            preventDefault: fakeFunc
        });

        sinon.assert.notCalled(spyTransition);
        sinon.assert.calledOnce(spyLoginService);
    }));

});