import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Cellular from './cellular.jsx';
import CellularSimStatus from '../../cellular/components/cellular-sim-status.jsx';
import CellularConfig from '../../cellular/components/cellular-config.jsx';
import {ModalContainer, ModalDialog} from 'react-modal-dialog';

describe('Cellular (Wizard) tests', function () {

    // Define some stubs for functionality used by components created under cellular.
    let sandbox, stubState, stubSetting;
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
        // Settings collection
        stubSetting = {
            findOne: sandbox.stub(),
            update:sandbox.stub()
        };  
        stubState.findOne.returns({_id: 1,sim_status: 'Using SIM1', signal_strength: "Poor", signal_quality: 'Excellent','ip_address': '10.52.18.176', _groupIndex: 1});
        stubSetting.findOne.returns({_id:"1",state: 'ON', apn: 1, _groupIndex: 1});

        deviceCtx = {
            SubsManager: stubSubsManager,
            Settings: stubSetting,
            State: stubState
        };
    });

    afterEach(()=> {
        sandbox.restore();
    });

    class CellularParent extends React.Component {
        getChildContext() {
            return {
                device: deviceCtx
            };
        }

        render(){
            return (
                <Cellular buttonConfig={sandbox.stub()}
                          disableNext={sandbox.stub()}
                          enableNext={sandbox.stub()}
                          transitionToNext={sandbox.stub()}
                          params="{_groupIndex:0}"/>
            )
        }
    }

    CellularParent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    it('should render cellular components', function () {
        let root = TestUtils.renderIntoDocument(<CellularParent/>);
        let cellular = TestUtils.findRenderedComponentWithType(root, Cellular);
        let cellularConfig = TestUtils.findRenderedComponentWithType(root, CellularConfig);
        let cellularSimStatus = TestUtils.findRenderedComponentWithType(root, CellularSimStatus);
        let header = TestUtils.findRenderedDOMComponentWithTag(cellularConfig, 'h6');
        expect(cellular).to.not.be.undefined;
        expect(cellularConfig).to.not.be.undefined;
        expect(cellularSimStatus).to.not.be.undefined;
        expect(header.textContent).to.equal('Specify Cellular Parameters');
    });

    it('should call buttonConfig, enableNext and not call transitionToNext when cellular is rendered',sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<CellularParent/>);
        let cellular = TestUtils.findRenderedComponentWithType(root, Cellular);
        sinon.assert.calledOnce(cellular.props.enableNext);
        sinon.assert.calledOnce(cellular.props.buttonConfig);
        sinon.assert.notCalled(cellular.props.transitionToNext);
    }));

    it('should start countdown timer on wizard next and not call transitionToNext',sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<CellularParent/>);
        let cellular = TestUtils.findRenderedComponentWithType(root, Cellular);
        let e = {bogus: 'event'};
        cellular.onVerify(e);
        let modalRetryDialog = TestUtils.findRenderedComponentWithType(cellular, ModalContainer);
        expect(modalRetryDialog).to.not.be.undefined;
        sinon.assert.notCalled(cellular.props.transitionToNext);
    }));

    it('should call update cellular settings with correct apn value after next button is clicked', sinon.test(function() {
        let root = TestUtils.renderIntoDocument(<CellularParent/>);
        let cellular = TestUtils.findRenderedComponentWithType(root, Cellular);
        let cellularConfig = TestUtils.findRenderedComponentWithType(cellular, CellularConfig);
        let selects = TestUtils.scryRenderedDOMComponentsWithTag(cellularConfig, 'select');
        let country = selects.find((select)=> {return select.name == 'cellular country'});
        let apn = selects.find((select)=> {return select.name == 'cellular APN'});
        let provider = selects.find((select)=> {return select.name == 'cellular provider'});
        let e = {bogus: 'event'};
        /*** Verify that by default when next is clicked the setting are called with correct apn value **/
        expect(country.value).to.equal('United Kingdom');
        expect(provider.value).to.equal('3');
        expect(apn.value).to.equal('3gpronto');
        cellular.onVerify(e);
        sinon.assert.calledOnce(stubSetting.update);
        sinon.assert.calledWithExactly(stubSetting.update,cellular.data.cellularIntSettings._id,{$set: {'apn': cellular.state.cellularAPN,'state': 'on'}});

        // Simulate country change and verify provider, and apn are changed
        TestUtils.Simulate.change(country, { target: { value: 'Angola' } });
        expect(country.value).to.equal('Angola');
        expect(provider.value).to.equal('Movicel Angola');
        expect(apn.value).to.equal('internet.movicel.co.ao');
        cellular.onVerify(e);
        /*** Verify that after apn is changed and when next is clicked the setting are called with correct apn value **/
        sinon.assert.calledWithExactly(stubSetting.update,cellular.data.cellularIntSettings._id,{$set: {'apn': cellular.state.cellularAPN,'state': 'on'}});
    }));
});


