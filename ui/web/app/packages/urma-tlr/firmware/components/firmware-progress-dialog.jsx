import {default as React, Component, PropTypes } from 'react';
import {ModalContainer, ModalDialog} from 'react-modal-dialog';
import Urma from 'meteor/digi:urma-core';
import { FW_EVENT } from '../firmware-constants';
import Reboot from './reboot.jsx';

let ProgressBar = Urma.ProgressBar;

const LOADING = 0;
const SUCCESS = 1;
const ERROR = 2;
const ABORT = 3;

export default class FirmwareProgressDialog extends Component {
    constructor(props) {
        super(props);
        this.state = {
            status: LOADING,
            progress: 0,
            message: '',
            error: ''
        };

        this.fnOnProgress = this.onProgress.bind(this);
        this.fnOnSuccess = this.onSuccess.bind(this);
        this.fnOnError = this.onError.bind(this);
        this.fnOnAborted = this.onAborted.bind(this);
    }

    componentWillMount() {
        // Set up listener for progress events.
        this.setState({progress: 0});
        this.props.firmwareManager
            .on(FW_EVENT.PROGRESS, this.fnOnProgress)
            .on(FW_EVENT.SUCCESS, this.fnOnSuccess)
            .on(FW_EVENT.ERROR, this.fnOnError)
            .on(FW_EVENT.ABORTED, this.fnOnAborted);
    }

    componentWillUnmount() {
        this.props.firmwareManager
            .removeListener(FW_EVENT.PROGRESS, this.fnOnProgress)
            .removeListener(FW_EVENT.SUCCESS, this.fnOnSuccess)
            .removeListener(FW_EVENT.ERROR, this.fnOnError)
            .removeListener(FW_EVENT.ABORTED, this.fnOnAborted);
    }

    onProgress(progress, message) {
        let state = {progress: progress, status: LOADING};
        if (message) {
            state.message = message;
        }
        this.setState(state);
    }

    onSuccess(message) {
        message = message || 'Firmware update complete';
        this.setState({message: message, progress: 100, status: SUCCESS});
    }

    onError(err) {
        this.setState({message: '', error: err.message, status: ERROR});
    }

    onAborted() {
        this.setState({message: 'Firmware update cancelled', status: ABORT});
    }

    handleClick(e) {
        if (this.state.status == LOADING) {
            this.props.firmwareManager.cancelUpdate(e);
        } else {
            this.props.onClose(e, this.state.status === SUCCESS);
        }
   }

    render() {
        let buttonText = '';
        let buttonDisabled = false;
        if (this.state.status == LOADING) {
            buttonText = 'Cancel';
            buttonDisabled = !this.props.canCancel;
        } else {
            buttonText = 'OK';
            buttonDisabled = false;
        }

        return (
            <ModalContainer>
                <ModalDialog>
                    {this.state.status === SUCCESS ?
                        <div style={{minWidth: '300px', maxWidth: '500px'}}>
                            <Reboot/>
                        </div>
                        :
                        <div style={{minWidth: '300px', maxWidth: '500px'}}>
                            <h5>Firmware Update</h5>
                            { this.state.error &&
                                <p className='error'>{this.state.error}</p>
                            }
                            <ProgressBar percent={this.state.progress} text={this.state.message}/>
                            <button type='button' style={{float: 'right'}} disabled={buttonDisabled}
                                onClick={this.handleClick.bind(this)}>
                                {buttonText}
                            </button>
                        </div>
                    }
                </ModalDialog>
            </ModalContainer>
        );
    }
}

FirmwareProgressDialog.propTypes = {
    firmwareManager: PropTypes.object.isRequired,
    onClose: PropTypes.func.isRequired,
    canCancel: PropTypes.bool
};
