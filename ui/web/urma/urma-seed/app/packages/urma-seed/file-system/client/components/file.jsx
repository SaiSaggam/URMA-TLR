import {Component, PropTypes} from 'react';

export default class File extends Component {

    isFileSelected(){
        this.props.selectFile(this.props.file);
    }

    setSelected(){
        if(!this.props.selectedFile){
            return false;
        } else if(this.props.file.name === this.props.selectedFile.name){
            return true;
        } else {
            return false;
        }
    }

    render(){
        return (
            <div className="file" onClick={this.isFileSelected.bind(this)} >
                <div className={this.setSelected() ? 'file-selected' : null}>
                    {this.props.file.type === 'file' ? <span>&nbsp;&nbsp;</span> : null}
                    <span className={this.props.file.type === 'file' ? 'fa fa-file' : 'fa fa-folder-open'}>&nbsp;</span>
                    <span className={this.props.file.type}>{this.props.file.fileSpec} - {this.props.file.type}</span>
                </div>
            </div>
        );
    }
}

File.propTypes = {
    file: React.PropTypes.object.isRequired,
    selectFile: React.PropTypes.func.isRequired,
    selectedFile: React.PropTypes.object
};