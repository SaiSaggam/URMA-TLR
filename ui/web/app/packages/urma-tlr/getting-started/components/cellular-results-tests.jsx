import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import CellularResults from './cellular-results.jsx';
import CellularSimStatus from '../../cellular/components/cellular-sim-status.jsx';

describe('Cellular Results (Wizard) tests', function () {

    // Define some stubs for functionality used by components created under cellular.
    let sandbox, stubState;
    let deviceCtx;
    beforeEach(()=> {
        sandbox = sinon.sandbox.create();
        // Meteor collection stubs
        let stubSubscriptionStub = { ready: sandbox.stub().returns(true)};
        let stubSubsManager = { subscribe: sandbox.stub().returns(stubSubscriptionStub) };

        // State collection
        stubState = {
            findOne: sandbox.stub()
        };
        stubState.findOne.returns({
            _id: 1,
            sim_status: 'Using SIM1',
            signal_strength: "Poor",
            signal_quality: 'Excellent',
            'ip_address': '10.52.18.176',
            _groupIndex: 1,
            description: "",
            module: "Sierra Wireless, Incorporated MC7354",
            firmware_version: "SWI9X15C_05.05.16.03",
            hardware_version: "1",
            imei: "359225050399534",
            registration_status: "Not registered",
            network_provider: "",
            temperature: "30",
            connection_type: "2G",
            radio_band: "",
            channel: "0",
            pdp_context: "",
            mask: "0.0.0.0",
            gateway: "0.0.0.0",
            dns_servers: "0.0.0.0",
            rx_packets: "0",
            tx_packets: "0",
            rx_bytes: "0",
            tx_bytes: "0"
        });
        deviceCtx = {
            SubsManager: stubSubsManager,
            State: stubState
        };
    });

    afterEach(()=> {
        sandbox.restore();
    });

    class CellularResultsParent extends React.Component {
        getChildContext() {
            return {
                device: deviceCtx
            };
        }

        render(){
            return (
                <CellularResults buttonConfig={sandbox.stub()}
                          disableNext={sandbox.stub()}
                          enableNext={sandbox.stub()}
                          transitionToNext={sandbox.stub()}/>
            )
        }
    }

    CellularResultsParent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    it('should render cellular results components', function () {
        let root = TestUtils.renderIntoDocument(<CellularResultsParent/>);
        let cellularResults = TestUtils.findRenderedComponentWithType(root, CellularResults);
        let cellularSimStatus = TestUtils.findRenderedComponentWithType(root, CellularSimStatus);
        let header = TestUtils.findRenderedDOMComponentWithTag(cellularResults, 'h6');
        expect(cellularResults).to.not.be.undefined;
        expect(cellularSimStatus).to.not.be.undefined;
        expect(header.textContent).to.equal('Cellular Connection Results');
    });

    it('should call buttonConfig, enableNext and not call transitionToNext when cellular results is rendered',sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<CellularResultsParent/>);
        let cellularResults = TestUtils.findRenderedComponentWithType(root, CellularResults);
        sinon.assert.calledOnce(cellularResults.props.enableNext);
        sinon.assert.calledOnce(cellularResults.props.buttonConfig);
        sinon.assert.notCalled(cellularResults.props.transitionToNext);
    }));

    it('verify all the state is being rendered correctly', function () {
        let root = TestUtils.renderIntoDocument(<CellularResultsParent/>);
        let cellularResults = TestUtils.findRenderedComponentWithType(root, CellularResults);
        let cellularResultsDisplay = TestUtils.findRenderedDOMComponentWithClass(cellularResults, 'wizard-content-area-right');
        let labels = TestUtils.scryRenderedDOMComponentsWithTag(cellularResults, 'label');
        let spans = TestUtils.scryRenderedDOMComponentsWithTag(cellularResults, 'span');

        expect(labels[4].textContent).to.equal('Description: ');
        expect(labels[5].textContent).to.equal('Module: ');
        expect(labels[6].textContent).to.equal('Firmware Version: ');
        expect(labels[7].textContent).to.equal('Hardware Version: ');
        expect(labels[8].textContent).to.equal('IMEI: ');
        expect(labels[9].textContent).to.equal('Registration Status: ');
        expect(labels[10].textContent).to.equal('Network Provider: ');
        expect(labels[11].textContent).to.equal('Temperature: ');
        expect(labels[12].textContent).to.equal('Connection Type: ');
        expect(labels[13].textContent).to.equal('Radio Band: ');
        expect(labels[14].textContent).to.equal('Channel: ');
        expect(labels[15].textContent).to.equal('PDP Context: ');
        expect(labels[16].textContent).to.equal('IP Address: ');
        expect(labels[17].textContent).to.equal('Mask: ');
        expect(labels[18].textContent).to.equal('Gateway: ');
        expect(labels[19].textContent).to.equal('DNS Servers: ');
        expect(labels[20].textContent).to.equal('RX Packets: ');
        expect(labels[21].textContent).to.equal('TX Packets: ');
        expect(labels[22].textContent).to.equal('RX Bytes: ');
        expect(labels[23].textContent).to.equal('TX Bytes: ');

        expect(spans[4].textContent).to.equal('');
        expect(spans[5].textContent).to.equal('Sierra Wireless, Incorporated MC7354');
        expect(spans[6].textContent).to.equal('SWI9X15C_05.05.16.03');
        expect(spans[7].textContent).to.equal('1');
        expect(spans[8].textContent).to.equal('359225050399534');
        expect(spans[9].textContent).to.equal('Not registered');
        expect(spans[10].textContent).to.equal('');
        expect(spans[11].textContent).to.equal('30');
        expect(spans[12].textContent).to.equal('2G');
        expect(spans[13].textContent).to.equal('');
        expect(spans[14].textContent).to.equal('0');
        expect(spans[15].textContent).to.equal('');
        expect(spans[16].textContent).to.equal('10.52.18.176');
        expect(spans[17].textContent).to.equal('0.0.0.0');
        expect(spans[18].textContent).to.equal('0.0.0.0');
        expect(spans[19].textContent).to.equal('0.0.0.0');
        expect(spans[20].textContent).to.equal('0');
        expect(spans[21].textContent).to.equal('0');
        expect(spans[22].textContent).to.equal('0');
        expect(spans[23].textContent).to.equal('0');
    });

});


