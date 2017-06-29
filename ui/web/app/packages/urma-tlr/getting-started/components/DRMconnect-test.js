import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import DRMconnect from './DRMconnect.jsx';

describe('DRMconnect (Wizard) tests', function () {

    // Define some stubs for functionality used by components created under DRMconnect.
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
            update: sandbox.stub()
        };
        stubSetting.findOne.returns({
            _id: 1,
            state: 'on',
            server: 'devtest.idigi.com'
        });
        stubState.findOne.returns({
            _id: 1,
            status: 'connected',
            server: "devtest.idigi.com",
            deviceid: '00000000-00000000-0040FFFF-FF0F44B0'
        });

        deviceCtx = {
            SubsManager: stubSubsManager,
            Settings: stubSetting,
            State: stubState
        };
    });

    afterEach(()=> {
            sandbox.restore();
    });

    class DRMconnectParent extends React.Component {
        getChildContext() {
            return {
                device: deviceCtx
            };
        }

        render(){
            return (
                <DRMconnect buttonConfig={sandbox.stub()}
                            disableNext={sandbox.stub()}
                            enableNext={sandbox.stub()}
                            transitionToNext={sandbox.stub()}/>
            )
        }
    }

    DRMconnectParent.childContextTypes = {
        device: React.PropTypes.object.isRequired
    };

    it('should render DRMconnect components', function () {
        let root = TestUtils.renderIntoDocument(<DRMconnectParent/>);
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        expect(inputs.length).to.equal(2);
        expect(inputs[0].getAttribute('type')).to.equal('text');
        expect(inputs[1].getAttribute('type')).to.equal('password');
    });

    it('should call dashboard on skip', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<DRMconnectParent/>);
        let drmConnect = TestUtils.findRenderedComponentWithType(root, DRMconnect);
        drmConnect.onSkip();
        sinon.assert.called(drmConnect.props.transitionToNext);
    }));

    it('should call dashboard on successful login', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<DRMconnectParent/>);
        let drmConnect = TestUtils.findRenderedComponentWithType(root, DRMconnect);
        let err;
        let result = {
            'result': 201
        }
        drmConnect.RMCallBackHandler(err,result);
        sinon.assert.called(drmConnect.props.transitionToNext);
    }));

    it('should call meteor method on next', sinon.test(function() {
        let stubMeteorCall = this.spy(Meteor, 'call');
        let root = TestUtils.renderIntoDocument(<DRMconnectParent/>);
        let drmConnect = TestUtils.findRenderedComponentWithType(root, DRMconnect);
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        let username = inputs.find((input)=> {return input.placeholder == "username"});
        let password = inputs.find((input)=> {return input.placeholder == "password"});
        TestUtils.Simulate.change(username, { target: { value: 'ssaggam' } });
        TestUtils.Simulate.change(password, { target: { value: '!ssaggam!' } });
        drmConnect.addDeviceToRemoteManger();
        sinon.assert.calledOnce(stubMeteorCall);
        sinon.assert.calledWith(stubMeteorCall,'provisionDevice','ssaggam','!ssaggam!',drmConnect.data.cloudDeviceId,drmConnect.data.cloudServer);
    }));
});