import {default as React, Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import reactMixin from 'react-mixin';
import FirmwareFileUpload from './firmware-file-upload.jsx';
import FirmwareProgressDialog from './firmware-progress-dialog.jsx';
import { FW_STATUS, FW_EVENT } from '../firmware-constants';

export default class FirmwareSelect extends React.Component {

    componentWillMount(){
        let mostRecentVersion = this.findMostRecentVersion(this.props.fwFiles);
        this.props.setInitialFwState(mostRecentVersion, this.upToDate());
    }

    upToDate() {
        return this.findMostRecentVersion(this.props.fwFiles).version === this.props.currentFwVersion;
    }

    findMostRecentVersion(fwFiles){
        let newestVersion = fwFiles[0];

        if(fwFiles.length === 1) {
            return newestVersion;
        } else {
            for(var fwIndex = 0; fwIndex < fwFiles.length; fwIndex++) {
                let version = fwFiles[fwIndex];

                let newestVersionArray = newestVersion.version.split('.');
                let versionArray = version.version.split('.');

                (function () {
                    for(var strIndex = 0; strIndex < newestVersionArray.length; strIndex++) {
                        let newestVersionInt = parseInt(newestVersionArray[strIndex]) || 0;
                        let versionInt = parseInt(versionArray[strIndex]) || 0;

                        if(versionInt > newestVersionInt) {
                            newestVersion = version;
                            return;
                        } else if (versionInt < newestVersionInt) {
                            return;
                        }
                    }
                })();

            }
            return newestVersion;
        }
    }

    render(){
        return (
            <div>
                {this.upToDate() ?
                    <div className="firmware-update-message"><p>The device's firmware is up to date</p></div>:
                    <div>
                        <form className="box">
                            <label className="bold">Available Version:</label>
                            <div className="select-box">
                                <select name="firmware versions" onChange={this.props.handleSelect}>
                                    {this.props.developmentMode ?
                                        this.props.fwFiles.map(fwVersion =>
                                            <option key={fwVersion.version} className="firmware-update-option-auto" value={fwVersion.name}>{fwVersion.version}</option>
                                        ):
                                        <option className="firmware-update-option-auto" value={this.findMostRecentVersion(this.props.fwFiles).name}>{this.findMostRecentVersion(this.props.fwFiles).version}</option>
                                    }
                                    <option className="firmware-update-option-manual" value="manual">Choose saved version</option>
                                </select>
                            </div>
                        </form>
                    </div>
                }
            </div>
        )
    }
}

FirmwareSelect.PropTypes = {
    fwFiles: PropTypes.object.isRequired,
    handleSelect: PropTypes.func.isRequired,
    setInitialFirmwareState: PropTypes.func.isRequired
};