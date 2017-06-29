import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import CelluarRetryDialog from '../../cellular/components/cellular-retry-dialog.jsx';
import {ModalContainer, ModalDialog,ModalPortal} from 'react-modal-dialog';
import Urma from 'meteor/digi:urma-core';

let CountdownTimer = Urma.CountdownTimer;
let fakeFunc = function () {};
let cellularIntState = {_id: 1,sim_status: 'Using SIM1', signal_strength: "Poor", signal_quality: 'Excellent','ip_address': '10.52.18.176', _groupIndex: 1} ;
let deviceSettings = {update:function () {}};

describe('Cellular Retry Dialog (Wizard) tests', function () {

    it('should render cellular retry dialog components', function () {
        let root = TestUtils.renderIntoDocument(<CelluarRetryDialog onClose={fakeFunc} onSuccess={fakeFunc} cellularAPN="" cellularIntState={cellularIntState} cellularSettingsId="0" deviceSettings={deviceSettings}/>);
        let buttons = TestUtils.scryRenderedDOMComponentsWithTag(root, 'button');
        let cancelBtn = buttons.find((button)=> {return button.id == 'cancelBtn'});
        let retryBtn = buttons.find((button)=> {return button.id == 'retryBtn'});
        expect(buttons.length).to.equal(1);
        expect(cancelBtn).to.not.be.undefined;
        expect(retryBtn).to.be.undefined;

        let header = TestUtils.findRenderedDOMComponentWithTag(root, 'h5');
        expect(header.textContent).to.equal('Please wait while your device connects.');

        let countdownTimer = TestUtils.findRenderedComponentWithType(root, CountdownTimer);
        expect(countdownTimer).to.not.be.undefined;
    });

    it('should render timeout message/Retry when timed out', function () {
        let root = TestUtils.renderIntoDocument(<CelluarRetryDialog onClose={fakeFunc} onSuccess={fakeFunc} cellularAPN="" cellularIntState={cellularIntState} cellularSettingsId="0" deviceSettings={deviceSettings}/>);
        root.setState({timedOut: true});
        let header = TestUtils.findRenderedDOMComponentWithTag(root, 'h5');
        let buttons = TestUtils.scryRenderedDOMComponentsWithTag(root, 'button');
        let cancelBtn = buttons.find((button)=> {return button.id == 'cancelBtn'});
        let retryBtn = buttons.find((button)=> {return button.id == 'retryBtn'});
        expect(buttons.length).to.equal(2);
        expect(cancelBtn).to.not.be.undefined;
        expect(retryBtn).to.not.be.undefined;
        expect(header.textContent).to.equal('Alert');
    });

    it('should call onClose function when cancel button is clicked', sinon.test(function () {
        let spyOnClose = this.spy();
        let spyonSuccess = this.spy();
        let root = TestUtils.renderIntoDocument(<CelluarRetryDialog onClose={spyOnClose} onSuccess={spyonSuccess} cellularAPN="" cellularIntState={cellularIntState} cellularSettingsId="0" deviceSettings={deviceSettings}/>);
        let buttons = TestUtils.scryRenderedDOMComponentsWithTag(root, 'button');
        let cancelBtn = buttons.find((button)=> {return button.id == 'cancelBtn'});
        TestUtils.Simulate.click(cancelBtn);
        sinon.assert.calledOnce(spyOnClose);
        sinon.assert.notCalled(spyonSuccess);
    }));

    it('should reset timers when retry button is clicked', sinon.test(function () {
        let spyOnClose = this.spy();
        let spyonSuccess = this.spy();
        let root = TestUtils.renderIntoDocument(<CelluarRetryDialog onClose={spyOnClose} onSuccess={spyonSuccess} cellularAPN="" cellularIntState={cellularIntState} cellularSettingsId="0" deviceSettings={deviceSettings}/>);
        root.setState({timedOut: true});
        let buttons = TestUtils.scryRenderedDOMComponentsWithTag(root, 'button');
        let retryBtn = buttons.find((button)=> {return button.id == 'retryBtn'});
        TestUtils.Simulate.click(retryBtn);
        sinon.assert.notCalled(spyOnClose);
        sinon.assert.notCalled(spyonSuccess);
        expect(root.state.timedOut).to.equal(false);
    }));

    it('if cellular connection is made/ if there is a ip_address onsucess func should be called', sinon.test(function () {
        let spyOnClose = this.spy();
        let spyonSuccess = this.spy();
        let root = TestUtils.renderIntoDocument(<CelluarRetryDialog onClose={spyOnClose} onSuccess={spyonSuccess} cellularAPN="" cellularIntState={cellularIntState} cellularSettingsId="0" deviceSettings={deviceSettings}/>);
        root.isConnected();
        sinon.assert.notCalled(spyOnClose);
        sinon.assert.calledOnce(spyonSuccess);
    }));

});


