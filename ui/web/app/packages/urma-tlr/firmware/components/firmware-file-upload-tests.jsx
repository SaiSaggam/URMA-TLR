import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import FirmwareFileUpload from './firmware-file-upload.jsx';
import { FirmwareManager } from '../firmware-manager.js';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';
import FileUploadProgress from 'react-fileupload-progress';

describe('FirmwareFileUpload', function () {
    let sandbox, xhr;
    let fwMgr = new FirmwareManager();

    beforeEach(()=> {
        sandbox = sinon.sandbox.create();
        // Fake requests for form submit of file upload.
        xhr = sandbox.useFakeXMLHttpRequest();
    });

    afterEach(() => {
        sandbox.restore();
        fwMgr.removeAllListeners();
    });

    it('should render file upload form', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr} />);
        let fup = TestUtils.findRenderedComponentWithType(root, FileUploadProgress);
        let form = TestUtils.findRenderedDOMComponentWithTag(fup, 'form');
        let input = TestUtils.findRenderedDOMComponentWithTag(fup, 'input');

        expect(fup).to.not.be.undefined;
        expect(form).to.not.be.undefined;
        expect(input.type).to.equal('file');
    });

    it('should set file upload url to firmware endpoint', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr} />);
        let fup = TestUtils.findRenderedComponentWithType(root, FileUploadProgress);
        expect(fup.props.url).to.equal('/firmware');
    });

    it('should include device ID in file upload url', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr}
                                                                    deviceID={'abcd1234'} />);
        let fup = TestUtils.findRenderedComponentWithType(root, FileUploadProgress);
        expect(fup.props.url).to.equal('/firmware/abcd1234');
    });

    it('should submit form on start fw update event', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr} />);
        expect(root.submitForm).to.exist;
        let e = {preventDefault: (e)=>{}}; // the react-fileupload-progress component calls e.preventDefault.
        fwMgr.startUpdate(e);
        expect(xhr.requests.length).to.equal(1);
    });

    it('should cancel file upload on cancel fw update event', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr} />);
        expect(root.cancelUpload).to.exist;
        let stubCancelUpload = sinon.stub(root, 'cancelUpload');
        let e = {bogus: 'event'};
        fwMgr.cancelUpdate(e);
        sinon.assert.calledOnce(stubCancelUpload);
        sinon.assert.calledWithExactly(stubCancelUpload, e);
    });

    it('should call onSelectFile when input file changes', function () {
        let spyOnSelectFile = sinon.spy();
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr}
                                                                    onSelectFile={spyOnSelectFile} />);
        let fileList = [{name: 'firmware.bin'}];
        let e = {target: {files: fileList}};
        root.onChangeFile(e);
        sinon.assert.calledOnce(spyOnSelectFile);
        sinon.assert.calledWithExactly(spyOnSelectFile, fileList);
    });

    it('should call file upload callbacks when provided', function () {
        let spyOnLoad = sinon.spy();
        let spyOnProgress = sinon.spy();
        let spyOnError = sinon.spy();
        let spyOnAbort = sinon.spy();
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr}
                                                                    onLoad={spyOnLoad}
                                                                    onProgress={spyOnProgress}
                                                                    onError={spyOnError}
                                                                    onAbort={spyOnAbort} />);
        let progress = 73;
        let e = {bogus: 'event'};
        let req = {bogus: 'request'};
        root.onLoad(e, req);
        root.onProgress(e, req, progress);
        root.onError(e, req);
        root.onAbort(e, req);
        sinon.assert.calledWithExactly(spyOnLoad, e, req);
        sinon.assert.calledWithExactly(spyOnProgress, e, req, progress);
        sinon.assert.calledWithExactly(spyOnError, e, req);
        sinon.assert.calledWithExactly(spyOnAbort, e, req);
    });

    it('should not fail when file upload callbacks not provided', function () {
        let root = TestUtils.renderIntoDocument(<FirmwareFileUpload firmwareManager={fwMgr} />);
        root.onLoad({}, {});
        root.onProgress({}, {}, 1);
        root.onError({}, {});
        root.onAbort({}, {});
    });

});
