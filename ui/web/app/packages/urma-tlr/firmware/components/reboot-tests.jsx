import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import Reboot from './reboot.jsx';

describe('Reboot Tests', function() {
    it('should display rebooting message just before device disconnects', sinon.test(function () {
        let stubUser = this.stub(Meteor, "status", ()=> {return {
            connected: 'true'
        }});
        let root = TestUtils.renderIntoDocument(
            <Reboot/>
        );
        let stubReconnect = this.stub(root, 'onReconnect', ()=>{});
        let rebootMsg = TestUtils.findRenderedDOMComponentWithTag(root, 'h5');

        expect(root.state.status).to.equal(1);
        expect(root.state.timedOut).to.equal(false);
        expect(rebootMsg.textContent).to.equal('Please wait while your device reboots.');
        sinon.assert.notCalled(stubReconnect);
    }));

    it('should display rebooting message when device disconnected', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(
            <Reboot/>
        );
        let stubReconnect = this.stub(root, 'onReconnect', ()=>{});
        root.setState({status: 2});

        let rebootMsg = TestUtils.findRenderedDOMComponentWithTag(root, 'h5');

        expect(rebootMsg.textContent).to.equal('Please wait while your device reboots.');
        sinon.assert.notCalled(stubReconnect);
    }));

    it('should display rebooting message just before device disconnects', sinon.test(function () {
        let stubUser = this.stub(Meteor, "status", ()=> {return {
            connected: 'true'
        }});
        let root = TestUtils.renderIntoDocument(
            <Reboot/>
        );
        root.rebootStatus = 3;
        root.setState({status: 3}); // force update

        let rebootMsg = TestUtils.findRenderedDOMComponentWithTag(root, 'h5');

        expect(rebootMsg.textContent).to.equal('Device connected. Initializing...');
    }));

    it('should call onReconnect when device ready', sinon.test(function () {
        let stubUser = this.stub(Meteor, "status", ()=> {return {
            connected: 'true'
        }});
        let root = TestUtils.renderIntoDocument(
            <Reboot/>
        );
        let stubReconnect = this.stub(root, 'onReconnect', ()=>{});
        root.rebootStatus = 4;
        root.setState({status: 4}); // force update

        sinon.assert.calledOnce(stubReconnect);
    }));

});