import React from 'react';
import {Router, Link, browserHistory} from 'react-router';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import FirmwareUpdateManual from './firmware-update-manual.jsx';
import FirmwareFileUpload from './firmware-file-upload.jsx';
import FirmwareProgressDialog from './firmware-progress-dialog.jsx';
import FirmwareSelect from './firmware-select.jsx';
import {FirmwareManager} from '../firmware-manager.js';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';
import ProgressMonitor from '../progress-monitor';

describe('FirmwareUpdateManual', function() {
    // Define some stubs for meteor collection stuff
    let sandbox, stubState, firmwareFiles;
    let spyOnReady, spyOnComplete;
    let deviceCtx;
    let stubFwMgr;
    let fwMgr = new FirmwareManager();

    // Parent class to contain FirmwareUpdateManual component and provide device context.
    class Parent extends React.Component {
        getChildContext() {
            return { device: deviceCtx };
        }
        render() {
            return (
                <FirmwareUpdateManual firmwareManager={fwMgr}
                    onReady={spyOnReady}
                    onComplete={spyOnComplete}/>
            );
        }
    }
    Parent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

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
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.4 3/18/2016 17:19:03'});
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns({_id: 1, status: 0, progress: 0});
        firmwareFiles = {
            _id: 1,
            available_images: {
                file: [
                    {date: '3/04/2016 7:45PM', name: 'lr54-1.2.3.4.bin', size: 23456780, version: '1.2.3.4'},
                    {date: '3/08/2016 4:33AM', name: 'lr54-1.2.3.5.bin', size: 23456890, version: '1.2.3.5'}
                ]
            }
        };
        stubState.findOne.withArgs({_groupName: 'firmware_files'}).returns(firmwareFiles);

        // Stub firmware manager functions
        stubFwMgr = {
            startUpdate: sandbox.stub(fwMgr, 'startUpdate'),
            cancelUpdate: sandbox.stub(fwMgr, 'cancelUpdate'),
            onProgress: sandbox.stub(fwMgr, 'onProgress'),
            onSuccess: sandbox.stub(fwMgr, 'onSuccess'),
            onError: sandbox.stub(fwMgr, 'onError'),
            onAborted: sandbox.stub(fwMgr, 'onAborted')
        }

        spyOnReady = sandbox.spy();
        spyOnComplete = sandbox.spy();
        deviceCtx = {
            env: 'device',
            SubsManager: stubSubsManager,
            State: stubState
        };
    });

    afterEach(()=> {
        sandbox.restore();
        fwMgr.removeAllListeners();
    });

    it('should render firmware select component', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let fwSelect = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareSelect);
        let selectComponent = TestUtils.findRenderedDOMComponentWithTag(fwSelect, 'select');

        expect(fwSelect).to.not.be.undefined;
        expect(selectComponent).to.not.be.undefined;
    }));

    it('should select manual and render manual firmware update component', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let fwSelect = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareSelect);
        let selectComponent = TestUtils.findRenderedDOMComponentWithTag(fwSelect, 'select');

        TestUtils.Simulate.change(selectComponent, { target: { value: 'manual' } });

        let fwFileUpload = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareFileUpload);

        expect(fwUpdateManual).to.not.be.undefined;
        expect(fwFileUpload).to.not.be.undefined;
        expect(fwUpdateManual.state.selectedFirmwareVersion).to.equal('manual');

        sinon.assert.calledTwice(spyOnReady);
        expect(spyOnReady.firstCall.args[0]).to.equal(true);
        expect(spyOnReady.secondCall.args[0]).to.equal(false);
    }));

    it('should select most manual then recent firmware version', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let fwSelect = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareSelect);
        let selectComponent = TestUtils.findRenderedDOMComponentWithTag(fwSelect, 'select');

        TestUtils.Simulate.change(selectComponent, { target: { value: 'manual' } });
        expect(fwUpdateManual.state.selectedFirmwareVersion).to.equal('manual');

        TestUtils.Simulate.change(selectComponent, { target: { value: 'lr54-1.2.3.5.bin' } });

        expect(fwUpdateManual.state.selectedFirmwareVersion).to.equal('lr54-1.2.3.5.bin');
        sinon.assert.calledThrice(spyOnReady);
        expect(spyOnReady.firstCall.args[0]).to.equal(true);
        expect(spyOnReady.secondCall.args[0]).to.equal(false);
        expect(spyOnReady.thirdCall.args[0]).to.equal(true);
    }));

    it('should call onReady when file selected', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Select file
        let fileList = [{name: 'firmware.bin'}];
        fwUpdateManual.onSelectFile(fileList);

        sinon.assert.calledTwice(spyOnReady);
        expect(spyOnReady.firstCall.args[0]).to.equal(true);
        expect(spyOnReady.secondCall.args[0]).to.equal(true);
    }));

    it('should call onReady when file unselected', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Unselect file
        let fileList = [];
        fwUpdateManual.onSelectFile(fileList);

        sinon.assert.calledTwice(spyOnReady);
        expect(spyOnReady.firstCall.args[0]).to.equal(true);
        expect(spyOnReady.secondCall.args[0]).to.equal(false);
    }));

    it('should call onReady when selected file list is null', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // null file list
        fwUpdateManual.onSelectFile(null);

        sinon.assert.calledTwice(spyOnReady);
        expect(spyOnReady.firstCall.args[0]).to.equal(true);
        expect(spyOnReady.secondCall.args[0]).to.equal(false);
    }));

    it('should call onComplete with success', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.handleCloseProgressDialog(e, true);
        sinon.assert.calledOnce(spyOnComplete);
        sinon.assert.calledWithExactly(spyOnComplete, true);
    }));

    it('should call onComplete with failure', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.handleCloseProgressDialog(e, false);
        sinon.assert.calledOnce(spyOnComplete);
        sinon.assert.calledWithExactly(spyOnComplete, false);
    }));

    it('should call update firmware method after file downloaded', sinon.test(function() {
        let stubMeteorCall = this.stub(Meteor, 'call');
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        let fileList = [{name: 'firmware.bin'}];
        fwUpdateManual.onSelectFile(fileList);
        fwUpdateManual.onLoad(e, {});

        // Expect a progress update then call to update firmware
        sinon.assert.calledOnce(stubFwMgr.onProgress);
        sinon.assert.calledOnce(stubMeteorCall);
        sinon.assert.calledWithExactly(stubMeteorCall, 'updateFirmware',
            'firmware.bin', {autoReboot: false, remoteLoad: false});
    }));

    it('should emit progress as file downloaded', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.onProgress(e, {}, 20);

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onProgress);
        sinon.assert.calledWithExactly(stubFwMgr.onProgress, sinon.match.number, sinon.match.string);
    }));

    it('should emit error if file upload fails', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.onError(e, {responseText: 'bummer'});

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onError);
        sinon.assert.calledWithExactly(stubFwMgr.onError, sinon.match.instanceOf(Error));
    }));

    it('should emit error with default text when no response text', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.onError(e, {});

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onError);
        sinon.assert.calledWithExactly(stubFwMgr.onError, sinon.match.instanceOf(Error));
    }));

    it('should emit aborted if file upload cancelled', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.onAbort(e, {});

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onAborted);
        sinon.assert.calledWithExactly(stubFwMgr.onAborted);
    }));

    it('should emit progress as auto firmware update downloads a file', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Override meteor state data with new firmware status
        let progress = 60;
        let fwStatus = {_id: 1, status: FW_STATUS.DOWNLOADING, progress: progress};
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns(fwStatus);

        fwUpdateManual.getMeteorData();

        // Currently progress monitoring assumes file download is half the processing.
        sinon.assert.calledOnce(stubFwMgr.onProgress);
        sinon.assert.calledWithExactly(stubFwMgr.onProgress, sinon.match.number, sinon.match.string);
    }));

    it('should emit progress as firmware image is being verified', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Override meteor state data with new firmware status
        let fwStatus = {_id: 1, status: FW_STATUS.VERIFYING, progress: 35};
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns(fwStatus);

        fwUpdateManual.firmwareUpdateProgress = 60;
        fwUpdateManual.getMeteorData();

        // The Applying step currently just updates progress by 5.
        sinon.assert.calledOnce(stubFwMgr.onProgress);
        sinon.assert.calledWithExactly(stubFwMgr.onProgress, sinon.match.number, sinon.match.string);
    }));

    it('should emit progress as firmware is being applied', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Override meteor state data with new firmware status
        let fwStatus = {_id: 1, status: FW_STATUS.APPLYING, progress: 35};
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns(fwStatus);

        fwUpdateManual.firmwareUpdateProgress = 50;
        fwUpdateManual.getMeteorData();

        // The Applying step currently just updates progress by 10.
        sinon.assert.calledOnce(stubFwMgr.onProgress);
        sinon.assert.calledWithExactly(stubFwMgr.onProgress, sinon.match.number, sinon.match.string);
    }));

    it('should emit complete when firmware update succeeds', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Override meteor state data with new firmware status
        let fwStatus = {_id: 1, status: FW_STATUS.COMPLETE};
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns(fwStatus);

        fwUpdateManual.getMeteorData();

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onSuccess);
        sinon.assert.calledWithExactly(stubFwMgr.onSuccess, sinon.match.string);
    }));

    it('should emit error when firmware update fails', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        // Override meteor state data with new firmware status
        let fwStatus = {_id: 1, status: FW_STATUS.ERROR, result: 'Ack! FW Update Failed!'};
        stubState.findOne.withArgs({_groupName: 'firmware_status'}).returns(fwStatus);

        fwUpdateManual.getMeteorData();

        // Expect a progress update with half progress
        sinon.assert.calledOnce(stubFwMgr.onError);
        sinon.assert.calledWithExactly(stubFwMgr.onError, sinon.match.instanceOf(Error));
    }));

    it('should parse available firmware files from firmware_files state', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        fwUpdateManual.getMeteorData();

        expect(fwUpdateManual.data.fwAvailableFiles).to.deep.equal(firmwareFiles.available_images.file);
    }));

    it('should show error and manual update form if no available versions are found', sinon.test(function () {

        firmwareFiles = {
            _id: 1,
            available_images: {
                file: []
            }
        };
        stubState.findOne.withArgs({_groupName: 'firmware_files'}).returns(firmwareFiles);

        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let error = TestUtils.findRenderedDOMComponentWithClass(fwUpdateManual, 'error');
        let fwFileUpload = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareFileUpload);


        expect(error).to.not.be.undefined;
        expect(error.textContent).to.equal('Error: Could not retrieve available firmware versions.');
        expect(fwFileUpload).to.not.be.undefined;
    }));

    it('should upToDate to false if update available', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let fwSelect = TestUtils.findRenderedComponentWithType(fwUpdateManual, FirmwareSelect);
        let selectComponent = TestUtils.findRenderedDOMComponentWithTag(fwSelect, 'select');
        
        expect(fwUpdateManual.state.selectedFirmwareVersion).to.equal('lr54-1.2.3.5.bin');
        expect(fwUpdateManual.state.upToDate).to.equal(false);

        sinon.assert.calledOnce(spyOnReady);
        sinon.assert.calledWith(spyOnReady, true);
    }));

    it('should set upToDate to true if firmware up to date', sinon.test(function () {
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.5 3/18/2016 17:19:03'});

        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        expect(fwUpdateManual.state.upToDate).to.equal(true);

        sinon.assert.calledOnce(spyOnReady);
        sinon.assert.calledWith(spyOnReady, true);
    }));

    it('should call onComplete and not firmwareUpdate if firmware up to date', sinon.test(function() {
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.5 3/18/2016 17:19:03'});

        let stubMeteorCall = this.stub(Meteor, 'call');
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let e = {bogus: 'event'};
        fwUpdateManual.startFirmwareUpdate(e);

        expect(fwUpdateManual.state.upToDate).to.equal(true);
        expect(fwUpdateManual.state.isShowingProgressDialog).to.equal(false);
        sinon.assert.notCalled(stubMeteorCall);
        sinon.assert.calledOnce(spyOnComplete);
        sinon.assert.calledWithExactly(spyOnComplete, true);
    }));

    it('should not call onComplete if firmware up to date but local file selected', sinon.test(function() {
        stubState.findOne.withArgs({_groupName: 'system'}).returns({_id: 1, firmware_version: '1.2.3.5 3/18/2016 17:19:03'});

        let stubMeteorCall = this.stub(Meteor, 'call');
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);

        let fileList = [{name: 'firmware.bin'}];
        fwUpdateManual.onSelectFile(fileList);
        let e = {bogus: 'event'};
        fwUpdateManual.startFirmwareUpdate(e);

        expect(fwUpdateManual.state.upToDate).to.equal(true);
        expect(fwUpdateManual.state.isShowingProgressDialog).to.equal(true);
        sinon.assert.notCalled(stubMeteorCall);
        sinon.assert.notCalled(spyOnComplete);
    }));

    it('should parse current firmware version', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<Parent/>);
        let fwUpdateManual = TestUtils.findRenderedComponentWithType(root, FirmwareUpdateManual);
        let currentVersion = TestUtils.findRenderedDOMComponentWithClass(fwUpdateManual, 'firmware-update-current-version');

        expect(currentVersion).to.not.be.undefined;
        expect(currentVersion.textContent).to.equal('Current Version: 1.2.3.4');

        let currentVersionObj = fwUpdateManual.parseCurrentFwVersion('1.2.3.5 3/18/2016 12:03:56');

        expect(currentVersionObj).to.deep.equal({
            currentVersion: '1.2.3.5',
            lastUpdateDay: '3/18/2016',
            lastUpdateTime: '12:03:56'
        });

        let currentVersionObjNoTime = fwUpdateManual.parseCurrentFwVersion('1.2.3.5 3/18/2016');

        expect(currentVersionObjNoTime).to.deep.equal({
            currentVersion: '1.2.3.5',
            lastUpdateDay: '3/18/2016',
            lastUpdateTime: null
        })
    }));
});
