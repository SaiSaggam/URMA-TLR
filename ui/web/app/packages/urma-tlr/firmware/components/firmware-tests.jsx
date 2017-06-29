import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Firmware from './firmware.jsx';
import FirmwareUpdateManual from './firmware-update-manual.jsx';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';

describe('Firmware', function () {
    // Define some stubs for functionality used by components created under Firmware.
    let sandbox, stubState;
    let deviceCtx;

    beforeEach(()=> {
        sandbox = sinon.sandbox.create();

        // Meteor collection stubs
        let stubSubscriptionStub = { ready: sandbox.stub().returns(true) };
        let stubSubsManager = { subscribe: sandbox.stub().returns(stubSubscriptionStub) };
        // State collection
        stubState = {
            findOne: sandbox.stub(),
            find: sandbox.stub()
        };
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.4'});
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns({_id: 1, status: 0, progress: 0});
        let stubFwFilesCursor = {fetch: sandbox.stub().returns([{_id: 1, name: 'fw1.bin', version: '1.2.3.5'}])};
        stubState.find.withArgs({_groupName: 'firmware_file'}).returns(stubFwFilesCursor);

        deviceCtx = {
            env: 'device',
            SubsManager: stubSubsManager,
            State: stubState
        };
    });

    afterEach(()=> {
        sandbox.restore();
    });

    class FirmwareParent extends React.Component {
        getChildContext() {
            return { device: deviceCtx };
        }
        render() {
            return (<Firmware/>)
        }
    }
    FirmwareParent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    it('should render manual firmware update components', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FirmwareParent/>);
        let firmware = TestUtils.findRenderedComponentWithType(root, Firmware);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(firmware, FirmwareUpdateManual);
        let header = TestUtils.findRenderedDOMComponentWithTag(firmware, 'h3');
        let updateButton = TestUtils.findRenderedDOMComponentWithTag(firmware, 'button');

        expect(firmware).to.not.be.undefined;
        expect(fwUpdateManual).to.not.be.undefined;
        expect(header.textContent).to.equal('Update Firmware');
        expect(updateButton.textContent).to.equal('Update Firmware');
        expect(updateButton.disabled).to.be.true; // disabled by default
    }));

    it('should toggle update button onready', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FirmwareParent/>);
        let firmware = TestUtils.findRenderedComponentWithType(root, Firmware);
        let updateButton = TestUtils.findRenderedDOMComponentWithTag(firmware, 'button');

        firmware.onReady(true);
        expect(updateButton.disabled).to.be.false;
        firmware.onReady(false);
        expect(updateButton.disabled).to.be.true;
    }));

    it('should start firware update on button click', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FirmwareParent/>);
        let firmware = TestUtils.findRenderedComponentWithType(root, Firmware);
        let updateButton = TestUtils.findRenderedDOMComponentWithTag(firmware, 'button');

        let stubStartEvent = this.stub(firmware.firmwareManager, "startUpdate");
        firmware.onReady(true);
        TestUtils.Simulate.click(updateButton);

        sinon.assert.calledOnce(stubStartEvent);
        sinon.assert.calledWithExactly(stubStartEvent, sinon.match.object/*event*/);
    }));

});
