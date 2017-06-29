import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import {FWUpdate} from './firmware-update.jsx';
import FirmwareUpdateManual from '../../firmware/components/firmware-update-manual.jsx';
import { FW_STATUS, FW_EVENT } from '../../firmware/firmware-constants';

describe('FWUpdate (Wizard)', function () {
    let stubState, deviceCtx;
    let router = {
        goBack: function () {}
    };

    class FWUpdateParent extends React.Component {
        getChildContext() {
            return {device: deviceCtx};
        }

        render() {
            // Simulate wizard adding properties for buttonConfig, disableNext, ...
            return (<FWUpdate
                router={router}
                buttonConfig={sinon.stub()}
                disableNext={sinon.stub()}
                enableNext={sinon.stub()}
                transitionToNext={sinon.stub()}
            />)
        }
    }
    FWUpdateParent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    beforeEach(()=> {
        // Meteor collection stubs
        let stubSubscriptionStub = { ready: sinon.stub().returns(true) };
        let stubSubsManager = { subscribe: sinon.stub().returns(stubSubscriptionStub) };
        // State collection
        stubState = {
            findOne: sinon.stub(),
            find: sinon.stub()
        };
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.4'});
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns({_id: 1, status: 0, progress: 0});
        let stubFwFilesCursor = {fetch: sinon.stub().returns([{_id: 1, name: 'fw1.bin', version: '1.2.3.5'}])};
        stubState.find.withArgs({_groupName: 'firmware_files'}).returns(stubFwFilesCursor);

        deviceCtx = {
            env: 'device',
            SubsManager: stubSubsManager,
            State: stubState
        };
    });


    it('should render manual firmware update components', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FWUpdateParent/>);
        let fwUpdate = TestUtils.findRenderedComponentWithType(root, FWUpdate);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(fwUpdate, FirmwareUpdateManual);
        let header = TestUtils.findRenderedDOMComponentWithTag(fwUpdate, 'h1');

        expect(fwUpdate).to.not.be.undefined;
        expect(fwUpdateManual).to.not.be.undefined;
        expect(header.textContent).to.equal('Firmware Update');
    }));

    it('should toggle wizard next button onready', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FWUpdateParent/>);
        let fwUpdate = TestUtils.findRenderedComponentWithType(root, FWUpdate);

        // Default when component mounts is to disable next button
        sinon.assert.calledOnce(fwUpdate.props.disableNext);
        fwUpdate.props.disableNext.reset();

        fwUpdate.onReady(true);
        sinon.assert.calledOnce(fwUpdate.props.enableNext);
        fwUpdate.onReady(false);
        sinon.assert.calledOnce(fwUpdate.props.disableNext);
    }));

    it('should start firmware update on wizard next', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FWUpdateParent/>);
        let fwUpdate = TestUtils.findRenderedComponentWithType(root, FWUpdate);

        sinon.assert.calledOnce(fwUpdate.props.buttonConfig);
        sinon.assert.calledWithExactly(fwUpdate.props.buttonConfig,
            sinon.match.func, sinon.match.string, sinon.match.func);

        let args = fwUpdate.props.buttonConfig.args[0];
        expect(args.length).to.equal(3);
        let onNextCB = args[2];

        let stubStartEvent = this.stub(fwUpdate.firmwareManager, "startUpdate");
        let e = {bogus: 'event'};
        onNextCB(e);

        sinon.assert.calledOnce(stubStartEvent);
        sinon.assert.calledWithExactly(stubStartEvent, sinon.match.object/*event*/);
    }));

    it('should transition to next wizard page on successful firmware update', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FWUpdateParent/>);
        let fwUpdate = TestUtils.findRenderedComponentWithType(root, FWUpdate);

        fwUpdate.onFirmwareUpdateComplete(true);
        sinon.assert.calledOnce(fwUpdate.props.transitionToNext);
    }));

    it('should remain on page on unsuccessful firmware update', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<FWUpdateParent/>);
        let fwUpdate = TestUtils.findRenderedComponentWithType(root, FWUpdate);

        fwUpdate.onFirmwareUpdateComplete(false);
        sinon.assert.notCalled(fwUpdate.props.transitionToNext);
    }));

});
