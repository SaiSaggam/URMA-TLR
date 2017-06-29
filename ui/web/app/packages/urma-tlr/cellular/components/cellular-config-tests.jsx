import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import CellularConfig from '../../cellular/components/cellular-config.jsx';

let fakeFunc = function () {};
let cellular_countries = {
    "Albania" : [
        { "provider" : "Vodafone AL",		"username" : "a",	 "password" : "a",	"APN" : [ "Twa" ] }
    ],
    "Algeria" : [
        { "provider" : "Djezzy",			"username" : "",	 "password" : "",	"APN" : [ "djezzy.internet", "djezzy.mms" ] },
        { "provider" : "Mobilis",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Nedjma",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Angola" : [
        { "provider" : "Movicel Angola",	"username" : "",	 "password" : "",	"APN" : [ "internet.movicel.co.ao" ] },
        { "provider" : "Unitel",			"username" : "",	 "password" : "",	"APN" : [ "internet.unitel.co.ao" ] },
        { "provider" : "Vodacom",			"username" : "",	 "password" : "",	"APN" : [ "vodanet" ] }
    ],
    "Argentina" : [
        { "provider" : "CTI",				"username" : "",	 "password" : "",	"APN" : [ "internet.ctimovil.com.ar", "mms.ctimovil.com.ar" ] },
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "igprs.claro.com.ar", "mms.ctimovil.com.ar" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.unifon", "mms.gprs.unifon.com.ar", "wap.gprs.unifon.com.ar", "mms.gprs.unifon.com.ar" ] },
        { "provider" : "Personal",			"username" : "",	 "password" : "",	"APN" : [ "gprs.personal.com" ]	}
    ],
    "United States" : [
        { "provider" : "AT&T",				"username" : "",	 "password" : "",	"APN" : [ "broadband", "wap.cingular" ],             "carrier" : "att" },
        { "provider" : "Sprint",			"username" : "",	 "password" : "",	"APN" : [], "fixedAPN" : "none",                     "carrier" : "sprint",  "gobiFirmware" : "2", "msl" : true },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "epc.tmobile.com" ] },
        { "provider" : "Verizon",			"username" : "",	 "password" : "",	"APN" : [ {"text": "VZWINTERNET", "value":"none"} ], "carrier" : "verizon", "gobiFirmware" : "1" }
    ],
    "Canada" : [
        { "provider" : "Bell",				"username" : "",	 "password" : "",	"APN" : [ "pda.bell.ca", "inet.bell.ca" ],              "carrier" : "bell" },
        { "provider" : "Fido",				"username" : "",	 "password" : "",	"APN" : [ "fido-core-appl1.apn", "internet.fido.ca", "isp.fido.apn" ] },
        { "provider" : "Koodo",				"username" : "",	 "password" : "",	"APN" : [ "sp.koodo.com" ] },
        { "provider" : "Mobilicity",		"username" : "",	 "password" : "",	"APN" : [ "wap.davewireless.com", "mms.davewireless.com" ] },
        { "provider" : "Rogers",			"username" : "",	 "password" : "",	"APN" : [ "rogers-core-appl1.apn", "isp.apn", "internet.com" ],              "carrier" : "rogers" },
        { "provider" : "Sasktel",			"username" : "",	 "password" : "",	"APN" : [ "inet.stm.sk.ca" ] },
        { "provider" : "Telus",				"username" : "",	 "password" : "",	"APN" : [ "sp.telus.com", "isp.telus.com" ],                          "carrier" : "telus" },
        { "provider" : "Wind",				"username" : "",	 "password" : "",	"APN" : [ "internet.windmobile.ca", "mms.windmobile.ca" ] }
    ]
}

describe('Cellular Config (Wizard) tests', function () {

    it('should render cellular config fields', function () {
        let root = TestUtils.renderIntoDocument(<CellularConfig onVerify={fakeFunc} callBackParent={fakeFunc} countryConfigData={cellular_countries} defaultCountry="Argentina"/>);
        let header = TestUtils.findRenderedDOMComponentWithTag(root, 'h6');
        let selects = TestUtils.scryRenderedDOMComponentsWithTag(root, 'select');
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');

        let country = selects.find((select)=> {return select.name == 'cellular country'});
        let apn = selects.find((select)=> {return select.name == 'cellular APN'});
        let provider = selects.find((select)=> {return select.name == 'cellular provider'});
        let username = inputs.find((input)=> {return input.placeholder == "Username"});
        let password = inputs.find((input)=> {return input.placeholder == "Password"});
        let simPin = inputs.find((input)=> {return input.placeholder == "N/A"});
        let customApn = inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});

        expect(selects.length).to.equal(3);
        expect(inputs.length).to.equal(3);
        expect(header.textContent).to.equal('Specify Cellular Parameters');
        expect(country).to.not.be.undefined;
        expect(apn).to.not.be.undefined;
        expect(provider).to.not.be.undefined;
        expect(username).to.not.be.undefined;
        expect(password).to.not.be.undefined;
        expect(simPin).to.not.be.undefined;
        expect(customApn).to.be.undefined;
    });
    
    it('should change provider,APN values based on country and provider ',function () {

        let root = TestUtils.renderIntoDocument(<CellularConfig onVerify={fakeFunc} callBackParent={fakeFunc} countryConfigData={cellular_countries} defaultCountry="Argentina"/>);
        let selects = TestUtils.scryRenderedDOMComponentsWithTag(root, 'select');
        let country = selects.find((select)=> {return select.name == 'cellular country'});
        let apn = selects.find((select)=> {return select.name == 'cellular APN'});
        let provider = selects.find((select)=> {return select.name == 'cellular provider'});
        expect(country.value).to.equal('Argentina');
        expect(provider.value).to.equal('CTI');
        expect(apn.value).to.equal('internet.ctimovil.com.ar');
        
        // Simulate country change and verify provider, and apn are changed
        TestUtils.Simulate.change(country, { target: { value: 'Angola' } });
        expect(country.value).to.equal('Angola');
        expect(provider.value).to.equal('Movicel Angola');
        expect(apn.value).to.equal('internet.movicel.co.ao');

        //change provider and verify apn value match the provider
        TestUtils.Simulate.change(provider, { target: { value: 'Vodacom' } });
        expect(apn.value).to.equal('vodanet');
    });

    it('show custom APN when country or provider is other and also when apn value is custom  else hide it',function () {

        let root = TestUtils.renderIntoDocument(<CellularConfig onVerify={fakeFunc} callBackParent={fakeFunc} countryConfigData={cellular_countries} defaultCountry="Albania"/>);
        let selects = TestUtils.scryRenderedDOMComponentsWithTag(root, 'select');
        let country = selects.find((select)=> {return select.name == 'cellular country'});
        let provider = selects.find((select)=> {return select.name == 'cellular provider'});
        let apn = selects.find((select)=> {return select.name == 'cellular APN'});
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let customApn = inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});
        
        expect(customApn).to.be.undefined;
        // set apn value to custom and verify custom apn is shown
        TestUtils.Simulate.change(apn, { target: { value: 'custom' } });
        inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        customApn= inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});
        expect(country.value).to.equal('Albania');
        expect(provider.value).to.equal('Vodafone AL');
        expect(customApn).to.be.undefined;

        // set provider value to other and verify custom apn is shown
        TestUtils.Simulate.change(provider, { target: { value: 'other' } });
        inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        customApn= inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});
        expect(country.value).to.equal('Albania');
        expect(customApn).to.not.be.undefined;
        
        // Simulate country change and verify provider, and apn
        TestUtils.Simulate.change(country, { target: { value: 'other' } });
        expect(provider.value).to.equal('other');
        expect(apn.value).to.equal('custom');
        //verify the custom apn is shown when country is other
        inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        customApn= inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});
        expect(customApn).to.not.be.undefined;
    });

    it('hide custom APN and disable APN when provider is verizon',function () {

        let root = TestUtils.renderIntoDocument(<CellularConfig onVerify={fakeFunc} callBackParent={fakeFunc} countryConfigData={cellular_countries} defaultCountry="United States"/>);
        let selects = TestUtils.scryRenderedDOMComponentsWithTag(root, 'select');
        let country = selects.find((select)=> {return select.name == 'cellular country'});
        let provider = selects.find((select)=> {return select.name == 'cellular provider'});
        let apn = selects.find((select)=> {return select.name == 'cellular APN'});
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let customApn = inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});

        expect(country.value).to.equal('United States');
        expect(provider.value).to.equal('AT&T');
        expect(apn.value).to.equal('broadband');
        expect(apn).to.not.be.undefined;
        expect(apn.disabled).to.be.false;
        expect(customApn).to.be.undefined;
        // set provider value to verizon and verify custom apn is shown
        TestUtils.Simulate.change(provider, { target: { value: 'Verizon' } });
        inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        customApn= inputs.find((input)=> {return input.placeholder == "Enter your custom APN here"});
        selects = TestUtils.scryRenderedDOMComponentsWithTag(root, 'select');
        apn = selects.find((select)=> {return select.name == 'cellular APN'});
        expect(apn).to.not.be.undefined;
        expect(apn.disabled).to.be.true;
        expect(customApn).to.be.undefined;

    });
    
});


