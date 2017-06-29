/**
 * Default country is based on user location if location cannot  be determined it defaults to United States
 *
 * User location is determined making use of jstz npm package (detects timezone) ,
 * 3stack:country-codes-tz meteor package (detects country code from timezone),
 * and List of countries from (ISO 3166-2) in ../../cellular/country-list.js file
 *
 * */

import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import CellularConfig from '../../cellular/components/cellular-config.jsx';
import CelluarRetryDialog from '../../cellular/components/cellular-retry-dialog.jsx';
import CellularSimStatus from '../../cellular/components/cellular-sim-status.jsx';
import { cellular_countries } from '../../cellular/cellular-constants.js';
import { country_list } from '../../cellular/country-list.js';
import jstz from 'jstz';
import {ModalContainer, ModalDialog} from 'react-modal-dialog';

export default class Cellular extends Component {
    constructor(props){
        super(props);
        this.state = {
            isShowingRetryDialog: false,
            cellularAPN:''
        };
    }

    getMeteorData() {
        let Settings = this.context.device.Settings;
        let State = this.context.device.State;
        let SubsManager = this.context.device.SubsManager;

        let cellularIntSettings = {_id:""};
        let cellularIntState = null;
        let search = {_groupName: 'cellular'};
        let searchSettings = {_groupName: 'cellular',_groupIndex: parseInt(this.props.params._groupIndex)};
        let settingsSubscription = SubsManager.subscribe('settings', search);
        let stateSubscription = SubsManager.subscribe('state', search);

        if (settingsSubscription.ready() && stateSubscription.ready()) {
            cellularIntSettings = Settings.findOne(searchSettings);
            cellularIntState = State.findOne(search);
        }

        return {
            cellularIntSettings: cellularIntSettings,
            cellularIntState: cellularIntState
        };
    }

    componentWillMount(){
        this.props.buttonConfig(
            '/getting-started/connect',
            '/getting-started/cellular-results',
            this.onVerify.bind(this));
        this.props.enableNext();
    }

    onCellularConfigChanged(newAPN) {
        this.setState({cellularAPN:newAPN});
    }

    onVerify(e) {
        /** TODO Need to add validation **/
        /** only updating APN may need to update other fields in future **/
        let cellularAPN = this.state.cellularAPN;
        let Settings = this.context.device.Settings;
        let cellularSettings = this.data.cellularIntSettings;
        Settings.update(cellularSettings._id, {$set: {'apn': cellularAPN,'state': 'on'}});
        this.setState({isShowingRetryDialog: true});
        return {
            transitionToNext: false
        };
    }

    handleCloseProgressDialog(e) {
        this.setState({isShowingRetryDialog: false});
    }

    getBrowserCountry(){
        /* Detect Browser Country, else we default to the "United States". */
        let country = "United States";
        let tz = jstz.determine().name();
        let countryCode = CountryCodesTz.countryCodeFromTz(tz);
        let countryArr = country_list.filter(
            function(country_list){return country_list.Code == countryCode}
        );
        if(countryArr[0] != undefined){
            country = countryArr[0].Name;
        }
        return country;
    }

    renderRetryDialog() {
        return (
            <ModalContainer>
                <ModalDialog>
                    <CelluarRetryDialog onSuccess={this.props.transitionToNext.bind(this)}
                                        onClose={this.handleCloseProgressDialog.bind(this)}
                                        cellularAPN = {this.state.cellularAPN}
                                        deviceSettings={this.context.device.Settings}
                                        cellularSettingsId={this.data.cellularIntSettings['_id']}
                                        cellularIntState = {this.data.cellularIntState}/>
                </ModalDialog>
            </ModalContainer>
        )
    }

    renderCellularConfig() {
        let country = this.getBrowserCountry();
        return (
            <CellularConfig onVerify={this.props.transitionToNext.bind(this)}
                            countryConfigData={cellular_countries}
                            defaultCountry={country}
                            callBackParent={this.onCellularConfigChanged.bind(this)}/>

        )
    }

    render() {
        return (
            <div>
                <div className="wizard-content-area-left">
                    <div className="wizard-images">
                        <img src="/images/LR54_frontPanel.png" alt="Front Panel"/>
                        {this.data.cellularIntState? 
                            <CellularSimStatus cellularIntState={this.data.cellularIntState}/> 
                            :
                            <p>Loading...</p>
                        }
                    </div>
                    <div className="wizard-content-text">
                        <p>Specify any required cellular parameters to connect.</p>
                        <p>Note that some parameters may be optional, such as APN username and password or the SIM PIN.</p>
                    </div>
                </div>
                <div className="wizard-content-area-right">
                    {this.renderCellularConfig()}
                    <div>
                        <br/>
                        <span className='float-right'>Click NEXT to verify.</span>
                    </div>
                    {
                        this.data.cellularIntState && this.state.isShowingRetryDialog && this.renderRetryDialog()
                    }
                </div>
            </div>
        );
    }
}

reactMixin(Cellular.prototype, ReactMeteorData);

Cellular.contextTypes = {
    device: PropTypes.object.isRequired
};