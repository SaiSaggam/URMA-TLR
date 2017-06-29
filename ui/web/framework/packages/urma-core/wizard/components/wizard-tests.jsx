import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import { UrmaTest } from 'meteor/digi:urma-core';

let Wizard = Urma.Wizard;
let WizardButtons = UrmaTest.WizardButtons;
let WizardLogout = UrmaTest.WizardLogout;

describe('Wizard Tests', function () {
    let sandbox, root, parent, stubUser, wizard, wizardbuttons, buttonLinks, nextButton, transitionToNextStub;

    let stubSubscriptionStub = { ready: sinon.stub().returns(true) };
    let stubSubsManager = { subscribe: sinon.stub().returns(stubSubscriptionStub) };
    let stubState = {
        findOne: sinon.stub()
    };
    stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, model: 'LR54'});


    let deviceCtx = {
        env: 'device',
        deviceID: '123',
        SubsManager: stubSubsManager,
        Settings: {},
        State: stubState,
        Files: {}
    };

    class Parent extends React.Component {
        getChildContext() {
            return {
                device: deviceCtx
            };
        }

        render(){
            return (
                <Wizard/>
            )
        }
    }

    Parent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    let routes = {
        path: '/',
        public: true,
        component: Parent,
        indexRoute: {
            name: 'wizard',
            public: true,
            component: Wizard
        }
    };

    let onNextFalse = ()=>{return {
        transitionToNext: false
    }};

    let onNextTrue = ()=>{return {
        transitionToNext: true
    }};

    let onNext = {
        onNextFalse: onNextFalse,
        onNextTrue: onNextTrue
    };

    beforeEach(function () {
        sandbox = sinon.sandbox.create();
        stubUser = sandbox.stub(Meteor, "user", ()=> {return {username: 'user'}});

        root = TestUtils.renderIntoDocument(<Router routes={routes} history={browserHistory}/>);
        parent = TestUtils.findRenderedComponentWithType(root, Parent);
        wizard = TestUtils.findRenderedComponentWithType(parent, Wizard);
        wizardbuttons = TestUtils.findRenderedComponentWithType(root, WizardButtons);
        nextButton = TestUtils.findRenderedDOMComponentWithClass(wizardbuttons, 'next');
        transitionToNextStub = sandbox.stub(Wizard.prototype, 'transitionToNext', ()=>{ return });
    });

    afterEach(function () {
        sandbox.restore();
    });

    it('should only have next links', function () {
        wizard.buttonConfig(null, '/nextpage');
        buttonLinks = TestUtils.scryRenderedDOMComponentsWithTag(wizardbuttons, 'button');

        expect(buttonLinks.length).to.equal(1);
        expect(buttonLinks[0].textContent).to.equal('Start Device Setup');

    });
    it('should have back and next links', function () {
        wizard.buttonConfig('/prevpage', '/nextpage');
        buttonLinks = TestUtils.scryRenderedDOMComponentsWithTag(wizardbuttons, 'button');

        expect(buttonLinks.length).to.equal(2);
        expect(buttonLinks[0].textContent).to.equal('Back');
        expect(buttonLinks[1].textContent).to.equal('Next');
    });
    it.skip('should have back and dashboard links', function () {
        wizard.buttonConfig('/prevpage', '/');
        buttonLinks = TestUtils.scryRenderedDOMComponentsWithTag(wizardbuttons, 'button');

        expect(buttonLinks.length).to.equal(2);
        expect(buttonLinks[0].textContent).to.equal('Back');
        expect(buttonLinks[1].textContent).to.equal('Dashboard');
    });
    it('should disable next', function () {
        wizard.disableNext();
        let button = TestUtils.scryRenderedDOMComponentsWithTag(wizardbuttons, 'button');
        expect(button[1].getAttribute('disabled')).to.exist;

        wizard.enableNext();
        expect(button[1].getAttribute('disabled')).to.not.exist;
    });
    it('should call onNext function but not transitionToNext', function () {
        let onNextFalseSpy = sandbox.spy(onNext, 'onNextFalse');

        wizard.buttonConfig('/prevpage', '/nextpage', onNext.onNextFalse);

        TestUtils.Simulate.click(nextButton);

        sinon.assert.calledOnce(onNextFalseSpy);
        expect(transitionToNextStub.callCount).to.equal(0);
    });
    it('should call onNext function and TransitionToNext', function () {
        let onNextTrueSpy = sandbox.spy(onNext, 'onNextTrue');

        wizard.buttonConfig('/prevpage', '/nextpage', onNext.onNextTrue);

        TestUtils.Simulate.click(nextButton);

        sinon.assert.calledOnce(onNextTrueSpy);
        sinon.assert.calledOnce(transitionToNextStub);

    });
    it('should call TransitionToNext', function () {
        wizard.buttonConfig('/prevpage', '/nextpage');

        TestUtils.Simulate.click(nextButton);

        sinon.assert.calledOnce(transitionToNextStub);
    });
    it('should show log out button if user logged in ', function () {
        let wizardLogout = TestUtils.findRenderedComponentWithType(wizard, WizardLogout);
        let logoutBtn = TestUtils.findRenderedDOMComponentWithTag(wizardLogout, 'button');

        let loginService = wizardLogout.loginService();
        let stubLogout = sandbox.stub(loginService, 'logout');

        expect(logoutBtn.textContent).to.equal('Logout');

        TestUtils.Simulate.click(logoutBtn);

        sinon.assert.calledOnce(stubLogout);
    });
});