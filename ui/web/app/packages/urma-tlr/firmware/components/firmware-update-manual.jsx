import {default as React, Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import reactMixin from 'react-mixin';
import FirmwareFileUpload from './firmware-file-upload.jsx';
import FirmwareProgressDialog from './firmware-progress-dialog.jsx';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';
import FirmwareSelect from './firmware-select.jsx';
import ProgressMonitor from '../progress-monitor';

let log = new Logger('firmware');

let mSettings = (Meteor.settings && Meteor.settings.public ? Meteor.settings.public : {});
let developmentMode = mSettings.development || false;

export default class FirmwareUpdateManual extends Component {
    constructor(props){
        super(props);
        this.fnStartFirmwareUpdate = this.startFirmwareUpdate.bind(this);
        this.fnOnFirmwareUpdateSuccess = this.onFirmwareUpdateSuccess.bind(this)
        this.progressMonitor = new ProgressMonitor(props.firmwareManager);
        this.state = {
            isFileLoaded: false,
            isShowingProgressDialog: false,
            selectedFirmwareVersion: 'manual',
            upToDate: false
        };
    }

    getMeteorData() {
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let system = null;
        let search = {_groupName: 'system'};
        let subscription = SubsManager.subscribe('state', search);
        if (subscription.ready()) {
            system = State.findOne(search);
        }

        let fwAvailableFiles = null;
        let fwFileSearch = {_groupName: 'firmware_files'};
        let fwFileSubscription = SubsManager.subscribe('state', fwFileSearch);
        if (fwFileSubscription.ready()) {
            // List of available firmware files/versions
            let fwf = State.findOne(fwFileSearch);
            if (fwf && fwf['available_images']) {
                fwAvailableFiles = fwf['available_images']['file'];
            } else {
                fwAvailableFiles = [];
            }
        }

        let fwStatus = null;
        let fwStatusSearch = {_groupName: 'firmware_status'};
        let fwStatusSubscription = SubsManager.subscribe('state', fwStatusSearch, {refreshInterval: 2});
        if (fwStatusSubscription.ready()) {
            // Handle progress of actual firmware update.
            fwStatus = State.findOne(fwStatusSearch);
            if (fwStatus) {
                this.progressMonitor.update(fwStatus.status, fwStatus.progress, fwStatus.result);
            }
        }

        return {
            system: system,
            fwAvailableFiles: fwAvailableFiles,
            fwStatus: fwStatus
        };
    }

    componentWillMount() {
        this.props.firmwareManager.on(FW_EVENT.START, this.fnStartFirmwareUpdate);
        this.props.firmwareManager.on(FW_EVENT.SUCCESS, this.fnOnFirmwareUpdateSuccess);
    }

    componentWillUnmount() {
        this.props.firmwareManager.removeListener(FW_EVENT.START, this.fnStartFirmwareUpdate);
        this.props.firmwareManager.removeListener(FW_EVENT.SUCCESS, this.fnOnFirmwareUpdateSuccess);
    }

    startFirmwareUpdate(e) {
        this.progressMonitor.reset();
        log.debug('start firmware update, this.selectedFile:', this.selectedFile)
        if(this.state.upToDate && !this.selectedFile) {
            this.props.onComplete(true);
        } else {
            this.setState({isFileLoaded: false});
            this.setState({isShowingProgressDialog: true});

            if (this.state.selectedFirmwareVersion !== 'manual') {
                Meteor.call('updateFirmware', this.state.selectedFirmwareVersion, {autoReboot: false, remoteLoad: true});
            }
        }
    }

    onFirmwareUpdateSuccess(e) {
        log.debug('calling reboot from firmware update manual');
        Meteor.call('reboot', 0);
    }

    handleCloseProgressDialog(e, successful) {
        this.setState({isShowingProgressDialog: false});
        if (this.props.onComplete) {
            this.props.onComplete(successful);
        }
    }

    onSelectFile(fileList) {
        log.debug('onSelectFile, fileList:', fileList, 'this.selectedFile:', this.selectedFile);
        this.selectedFile = (fileList && fileList.length > 0 ? fileList[0].name : null);
        this.props.onReady(!!this.selectedFile);
    }

    //
    // File upload progress callbacks
    //
    onLoad(e, request) {
        this.setState({isFileLoaded: true});
        this.progressMonitor.update(FW_STATUS.DOWNLOADING, 100);
        Meteor.call('updateFirmware', this.selectedFile, {autoReboot: false, remoteLoad: false});
    }

    onProgress(e, request, progress) {
        this.progressMonitor.update(FW_STATUS.DOWNLOADING, progress);
    }

    onError(e, request) {
        let text = request.responseText || 'Unknown error.';
        let message = 'Failed to download firmware - '+text;
        this.progressMonitor.update(FW_STATUS.ERROR, 100, message);
    }

    onAbort(e, request) {
        this.props.firmwareManager.onAborted();
    }

    renderProgressDialog() {
        // Only allow abort during file download. Can't abort during device firmware update.
        let canCancel;

        if(this.state.selectedFirmwareVersion === 'manual'){
            canCancel = !this.state.isFileLoaded;
        } else {
            canCancel = false;
        }
        
        return (
            <FirmwareProgressDialog firmwareManager={this.props.firmwareManager}
                                    canCancel={canCancel}
                                    onClose={this.handleCloseProgressDialog.bind(this)} />
        )
    }

    parseCurrentFwVersion(currentFw){
        let currentFwArray = currentFw.split(' ');
        return {
            currentVersion: currentFwArray[0],
            lastUpdateDay: currentFwArray[1] || null,
            lastUpdateTime: currentFwArray[2] || null
        };
    }

    handleSelect(e){
        this.setState({selectedFirmwareVersion: e.target.value}, function () {
            let selectManual = (this.state.selectedFirmwareVersion === 'manual');
            log.debug('handleSelect: manual:', selectManual, 'this.selectedFile:', this.selectedFile);
            if (selectManual) {
                this.props.onReady(false);
            } else {
                this.props.onReady(true);
            }
        });
    }

    setInitialFwState(mostRecentFw, upToDate){
        log.debug('setInitialFwState mostRecetFw:', mostRecentFw, 'upToDate:', upToDate, 'this.selectedFile:', this.selectedFile);
        if(upToDate) {
            this.setState({upToDate: true}, function () {
                this.props.onReady(true);
            })
        } else {
            this.setState({selectedFirmwareVersion: mostRecentFw.name}, function () {
                this.props.onReady(true);
            })
        }
    }

    renderFwSelect(fwAvailableFiles, system) {
        if(!fwAvailableFiles || !system) {
            return (
                <div>Loading...</div>
            )
        } else if (fwAvailableFiles && system && !fwAvailableFiles.length) {
            return (
                <div>
                    <p className="error"><span className="bold">Error: </span>
                        Could not retrieve available firmware versions.</p>
                </div>
            )
        } else {
            return (
                <FirmwareSelect
                    developmentMode={developmentMode}
                    setInitialFwState={this.setInitialFwState.bind(this)}
                    handleSelect={this.handleSelect.bind(this)}
                    fwFiles={fwAvailableFiles}
                    currentFwVersion={this.parseCurrentFwVersion(system['firmware_version']).currentVersion}
                    ></FirmwareSelect>
            )
        }
    }

    render() {
        return (
            <div>
                <div>
                        <div>
                            {this.data.system ?
                                <div className="box firmware-update-current-version">
                                    <label className="bold">Current Version:</label>
                                    <span> {this.parseCurrentFwVersion(this.data.system['firmware_version']).currentVersion}</span>
                                </div>
                                :
                                <div>Loading...</div>
                            }
                            {
                                this.renderFwSelect(this.data.fwAvailableFiles, this.data.system)
                            }
                            {
                                this.state.selectedFirmwareVersion === 'manual' || (this.data.fwAvailableFiles && !this.data.fwAvailableFiles.length) ?
                                    <div className="firmware-update-manual margin-top">
                                        <p>Download and install a firmware file from your computer.</p>
                                        <FirmwareFileUpload firmwareManager={this.props.firmwareManager}
                                                            deviceID={this.context.device.deviceID}
                                                            onSelectFile={this.onSelectFile.bind(this)}
                                                            onProgress={this.onProgress.bind(this)}
                                                            onLoad={this.onLoad.bind(this)}
                                                            onError={this.onError.bind(this)}
                                                            onAbort={this.onAbort.bind(this)}
                                        />
                                    </div>:
                                    null
                            }
                        </div>
                </div>


                {
                    this.state.isShowingProgressDialog &&
                    this.renderProgressDialog()
                }

            </div>
        );
    }
}

reactMixin(FirmwareUpdateManual.prototype, ReactMeteorData);

FirmwareUpdateManual.propTypes = {
    firmwareManager: PropTypes.object.isRequired,
    onReady: PropTypes.func.isRequired,
    onComplete: PropTypes.func
};

FirmwareUpdateManual.contextTypes = {
    device: PropTypes.object.isRequired
};
