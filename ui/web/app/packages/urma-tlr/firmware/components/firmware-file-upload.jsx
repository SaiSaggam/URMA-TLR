'use strict';

import {default as React, Component, PropTypes } from 'react';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';
import FileUploadProgress from 'react-fileupload-progress';

export default class FirmwareFileUpload extends Component {
    constructor(props) {
        super(props);

        this.fnSubmitFirmwareUpdate = this.submitFirmwareUpdate.bind(this);
        this.fnAbortFirmwareUpload = this.abortFirmwareUpload.bind(this);
    }

    componentWillMount() {
        this.props.firmwareManager
            .on(FW_EVENT.START, this.fnSubmitFirmwareUpdate)
            .on(FW_EVENT.CANCEL, this.fnAbortFirmwareUpload);
    }
    componentWillUnmount() {
        this.props.firmwareManager
            .removeListener(FW_EVENT.START, this.fnSubmitFirmwareUpdate)
            .removeListener(FW_EVENT.CANCEL, this.fnAbortFirmwareUpload);
    }

    /**
     * Public function to initiate form submit externally (such as via the wizard Next button).
     * @param e the event that initiated this action
     */
    submitFirmwareUpdate(e) {
        this.submitForm(e);
    }

    abortFirmwareUpload(e) {
        //this.refs.fileUploader.cancelUpload();
        this.cancelUpload(e);
    }

    onChangeFile(e) {
        if (this.props.onSelectFile) {
            this.props.onSelectFile(e.target.files);
        }
    }

    onLoad(e, request) {
        if (this.props.onLoad) {
            this.props.onLoad(e, request);
        }
    }

    onProgress(e, request, progress) {
        if (this.props.onProgress) {
            this.props.onProgress(e, request, progress);
        }
    }

    onError(e, request) {
        if (this.props.onError) {
            this.props.onError(e, request);
        }
    }

    onAbort(e, request) {
        if (this.props.onAbort) {
            this.props.onAbort(e, request);
        }
    }

    formGetter() {
        // Unfortunately the form gets rendered from the FileUploadProgress component so we can't just define
        // a 'ref' to the form (because we don't own it). Look it up by ID to create the FormData.
        let formdata = new FormData(document.getElementById('fwFileUploadForm'));
        return formdata;
    }

    fwUploadFormRenderer(onSubmit) {
        // Save the submit function so that we can call it programmatically.
        this.submitForm = onSubmit;
        return (
            <form id='fwFileUploadForm' name='fwfileupload' encType='multipart/form-data' method='post'
                  onSubmit={onSubmit}>
                <input type='file' name='files[]' onChange={this.onChangeFile.bind(this)} />
            </form>
        )
    }

    fwUploadProgressRenderer(progress, hasError, cancelHandler) {
        this.cancelUpload = cancelHandler;
        // We use a modal dialog to show full firmware update progress, not just file upload. Don't render anything.
        return;
    }

    render() {
        let fwUrl = '/firmware';
        if (this.props.deviceID) {
            fwUrl += ('/'+this.props.deviceID);
        }

        return (
            <div className="firmware-update-choose-file">
                <FileUploadProgress url={fwUrl}
                                    firmwareManager={this.props.firmwareManager}
                                    onProgress={this.onProgress.bind(this)}
                                    onLoad={this.onLoad.bind(this)}
                                    onError={this.onError.bind(this)}
                                    onAbort={this.onAbort.bind(this)}
                                    formGetter={this.formGetter.bind(this)}
                                    formRenderer={this.fwUploadFormRenderer.bind(this)}
                                    progressRenderer={this.fwUploadProgressRenderer.bind(this)}
                    />
            </div>
        )
    }
};

FirmwareFileUpload.propTypes = {
    firmwareManager: PropTypes.object.isRequired,
    onSelectFile: PropTypes.func,
    deviceID: PropTypes.string,
    onProgress: PropTypes.func,
    onLoad: PropTypes.func,
    onError: PropTypes.func,
    onAbort: PropTypes.func
};
