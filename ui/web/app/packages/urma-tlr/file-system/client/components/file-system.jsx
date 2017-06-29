import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import File from './file.jsx';

export default class FileSystem extends Component {
    constructor(props){
        super(props);
        this.state = {
            selectedFile: null
        };
    }

    getMeteorData() {
        let Files = this.context.device.Files;
        let SubsManager = this.context.device.SubsManager;

        let files = {};
        let subscriptionReady = false;
        let sort = {fileSpec: 1, type: 1};
        let subscription = SubsManager.subscribe('files', {}, {sort: sort});
        if(subscription.ready()){
            files = Files.find({}, sort).fetch();
            subscriptionReady = true;
        }

        return {
            files: files,
            subscriptionReady: subscriptionReady
        }
    }

    selectFile(file){
        this.setState({selectedFile: file});
    }

    submitFileUpload(e) {
        var formData = new FormData(e.target);
        //formData.append('filePath', selectedPath());     NOTE: We don't need to do this now since we add the path to the upload URL
        //// TODO: consider sending additional formdata such as file sizes for per-file upload progress tracking
        ////var formData = new FormData();
        ////for (var i=0; i<files.length; i++) {
        ////  formData.append('fileSizes[]', file[i].size)
        ////  formData.append('files[]', file[i])
        ////}
        var xhr = new XMLHttpRequest();
        xhr.open('POST', '/uploads'+this._selectedPath(), true);
        xhr.onload = function(event){
            console.log('file loaded', event);
        };

        xhr.send(formData);
    }

    _selectedPath(includeTrailingSlash) {
        var path = '';
        var selectedFile = this.state.selectedFile;

        if (!!selectedFile) {
            path = selectedFile.path;
            if (selectedFile.type=='directory') {
                path += selectedFile.name;
            }
        }
        if (!!includeTrailingSlash) {
            if (path[path.length-1]!=='/') {
                path+='/';
            }
        } else {
            if (path[path.length-1]==='/') {
                path = path.substr(0, path.length-1);
            }
        }
        return path;
    }

    renderFilesList(){
        let files = this.data.files;
        return files.map(file =>
            <File key={file._id} file={file} selectFile={this.selectFile.bind(this)} selectedFile={this.state.selectedFile}/>);
    }

    render() {
        return (
            <div>
                { this.data.subscriptionReady ? this.renderFilesList() : null }
                <h3>Upload Files</h3>
                <form encType="multipart/form-data" method="post" name="fileinfo" onSubmit={this.submitFileUpload.bind(this)}>
                    <label>File to Upload:</label>
                    <input type="file" name="files[]"/>
                    <button type="submit">Upload the File!</button>
                </form>
            </div>
        );
    }
}

reactMixin(FileSystem.prototype, ReactMeteorData);

FileSystem.contextTypes = {
    device: PropTypes.object.isRequired
};
