/***
 * Country information needs to be passed into the CellularConfig component as an object from ../cellular-constants.js file .
 * and also Default country needs to be passed in
 *
 * User location can be determined by making use of jstz npm package (detects timezone) ,
 * 3stack:country-codes-tz meteor package (detects country code from timezone),
 * and List of countries from (ISO 3166-2) in ../country-list.js file . For example getting-started/components/cellular.jsx
 *
 */

import {default as React, Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';
import LinkedStateMixin from 'react-addons-linked-state-mixin';

export default class CellularConfig extends Component {
    constructor(props){
        super(props);
        this.state = {
            selectedCountry: this.props.defaultCountry,
            selectedProvider: 0,
            selectedAPN: 0
        };
    }

    componentWillMount(){
        this.providerDefaultValue = '';
        this.apnDefaultValue = '';
    }

    componentDidMount(){
        this.callBackParent();
    }

    componentDidUpdate (){
        this.callBackParent();
    }

    callBackParent(){
        /** setting apn and provider state only when country is changed and provider is not selected ,
         * when provider is changed and no apn is selected .  (Default values of cellularProvider and cellularAPN
         */
        if(this.state.selectedProvider === 0){
            let provider = this.providerDefaultValue;
            this.setState({selectedProvider: provider});
        }
        if(this.state.selectedAPN === 0){
            let apn = this.apnDefaultValue;
            this.setState({selectedAPN: apn});
            if(apn === "custom"){
                apn="";
            }
            this.props.callBackParent(apn);
        }
    }

    handleCountrySelect(e){
        this.setState({selectedCountry: e.target.value,
                       selectedProvider: 0,
                       selectedAPN: 0});
    }

    handleProviderSelect(e){
        this.setState({selectedProvider: e.target.value,selectedAPN:0});
    }

    handleAPNSelect(e){
        let apn = "";
        if(e.target.id === "cellularAPN"){
            this.setState({selectedAPN: e.target.value});
            if(e.target.value !== "custom"){
                apn = e.target.value ;
            }
        }else{
            apn = e.target.value;
        }
        this.props.callBackParent(apn);
    }

    renderProvider(){
        var rows = [];
        let disableProvider = false;
        if(this.state.selectedCountry !== "other"){
            let cellularData = this.props.countryConfigData[this.state.selectedCountry];
            this.providerDefaultValue = cellularData[0]['provider'];
            for (var i=0; i < cellularData.length; i++) {
                rows.push(<option key={cellularData[i]['provider']}  value={cellularData[i]['provider']}>{cellularData[i]['provider']}</option>);
            }
        }else{
            disableProvider = true;
            this.providerDefaultValue = "other";
        }

        rows.push(<option key={this.state.selectedCountry + "_other"}  value="other">Other</option>);

        return (
            <div id="cellular_provider_row" className="formRow">
                <label>Provider:</label>
                <div className="select-box">
                    <select name="cellular provider" id="cellularProvider" disabled={disableProvider} onChange={this.handleProviderSelect.bind(this)} >
                        {rows}
                    </select>
                </div>
            </div>
        );
    }

    renderAPN() {
        let rows = [];
        let disableAPN = false;
        /* if Provider is Verizon, hide APN*/
        if(this.state.selectedProvider === "Verizon"){
            rows.push(<option key="N/A" value="">N/A</option>);
            disableAPN = true;
            this.apnDefaultValue = "";
        }
        else {
            this.apnDefaultValue = "";
            if (this.state.selectedCountry !== "other" && this.state.selectedProvider !== "other" ) {
                let cellularData = this.props.countryConfigData[this.state.selectedCountry];
                /* No Provider is selected, display APN for first Provider in list*/
                if(this.state.selectedProvider === 0){
                    let arrAPN = cellularData[0]['APN'];
                    if(arrAPN.length > 0){
                        this.apnDefaultValue = arrAPN[0];
                    }
                    for (var j = 0; j < arrAPN.length; j++) {
                        rows.push(<option key={arrAPN[j]} value={arrAPN[j]}>{arrAPN[j]}</option>);
                    }
                }else{
                    for (var i = 0; i < cellularData.length; i++) {
                        if (cellularData[i]['provider'] === this.state.selectedProvider) {
                            let arrAPN = cellularData[i]['APN'];
                            if(arrAPN.length > 0){
                                this.apnDefaultValue = arrAPN[0];
                            }
                            for (var j = 0; j < arrAPN.length; j++) {
                                rows.push(<option key={arrAPN[j]} value={arrAPN[j]}>{arrAPN[j]}</option>);
                            }
                            break;
                        }
                    }
                }
            }
            /** if there are no apn values for selected provider then custom will be the only apn set it as default**/
            if(this.apnDefaultValue === ""){
                this.apnDefaultValue = "custom";
                disableAPN = true;
            }
            rows.push(<option key={this.state.selectedCountry + this.state.selectedProvider + "_custom"} value="custom">Custom</option>);
        }

        return (
            <div id="cellular_APN_row" className="formRow">
                <label>APN</label>
                <div className="select-box">
                    <select name="cellular APN" id="cellularAPN" disabled={disableAPN} onChange={this.handleAPNSelect.bind(this)}>
                        {rows}
                    </select>
                </div>
            </div>
        );
    }
    
    renderCustomAPN() {
        return (
            <div className="formRow">
                <label></label>
                <div className="input-box">
                    <input type="text" placeholder="Enter your custom APN here" maxLength="63" id="cellularCustomAPN" onChange={this.handleAPNSelect.bind(this)} />
                </div>
            </div>
        );
    }
    
    render() {
        return (
            <div>
                <h6>Specify Cellular Parameters</h6>
                <form className="cellularConfigForm" id="cellularConfigForm" method='post'>
                    <div className="formRow">
                        <label>Country</label>
                        <div className="select-box">
                            <select name="cellular country" id="cellularCountry" onChange={this.handleCountrySelect.bind(this)} value={this.state.selectedCountry}>
                                { Object.keys(this.props.countryConfigData).map(cellularCountry =>
                                    <option key={cellularCountry} value={cellularCountry}>{cellularCountry}</option>
                                )}
                                <option key="other" value="other">Other</option>
                            </select>
                        </div>
                    </div>

                    {this.renderProvider()}
                    {this.renderAPN()}
                    {(this.state.selectedAPN === "custom" || this.state.selectedCountry === "other" || this.state.selectedProvider === "other" ) ?
                      this.renderCustomAPN():
                      null
                    }

                    <div className="formRow">
                        <label></label>
                        <div className="input-box">
                            <input type="text" id="cellularUsername" placeholder="Username" maxLength="63"/>
                         </div>
                    </div>
                    <div className="formRow">
                        <label></label>
                        <div className="input-box">
                            <input type="password" id="cellularPassword" placeholder="Password" maxLength="128"/>
                         </div>
                    </div>
                    <div className="formRow">
                        <label>SIM PIN</label>
                        <div className="input-box">
                            <input type="text" id="cellularSIMPin"  placeholder="N/A" maxLength="4"/>
                        </div>
                    </div>
                </form>
            </div>
        );
    }
}

reactMixin(CellularConfig.prototype, LinkedStateMixin);

CellularConfig.propTypes = {
    onVerify: PropTypes.func.isRequired,
    countryConfigData: PropTypes.object.isRequired,
    defaultCountry: PropTypes.string.isRequired,
    callBackParent: PropTypes.func.isRequired
};