import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Connect from './connect.jsx';


let fakeFunc = function () {};

describe('Connect (Wizard) tests', function () {

    it('should render connect components', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(
            <Connect buttonConfig={fakeFunc} disableNext= {fakeFunc}  enableNext={fakeFunc} transitionToNext= {fakeFunc} loggedIn={true} />
        );
        let header = TestUtils.findRenderedDOMComponentWithTag(root, 'h1');

        expect(header.textContent).to.equal('Choose to connect with Ethernet or cellular.');

        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');

        expect(inputs.length).to.equal(2);
        expect(inputs[0].getAttribute('type')).to.equal('radio');
        expect(inputs[1].getAttribute('type')).to.equal('radio');
        
    }));

    it('should call buttonConfig and enableNext when connect is rendered', sinon.test(function () {
        let spyTransition = this.spy();
        let spyButtonConfig = this.spy();
        let spyEnableNext = this.spy();
        let root = TestUtils.renderIntoDocument(
            <Connect loggedIn={true} enableNext={spyEnableNext} buttonConfig={spyButtonConfig} transitionToNext={spyTransition}/>
        );

         sinon.assert.calledOnce(spyButtonConfig);
         sinon.assert.calledOnce(spyEnableNext);
         sinon.assert.notCalled(spyTransition);

    }));

    it ("should have input for Select WAN connection type, verify options is checked ", function() {
        let root = TestUtils.renderIntoDocument(
            <Connect buttonConfig={fakeFunc} disableNext= {fakeFunc} enableNext={fakeFunc} transitionToNext= {fakeFunc} loggedIn={true}/>
        );
        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        expect(inputs.length).to.equal(2);
        expect(inputs[1].hasAttribute('defaultChecked'));
        expect(inputs[0].getAttribute('value')).to.equal('Cellular');
        expect(inputs[1].getAttribute('value')).to.equal('Ethernet');
 
    });

    it("Should state updates on selecting different option", sinon.test(function(){
        let spyButtonConfig = this.spy();
        let root = TestUtils.renderIntoDocument(
            <Connect buttonConfig={spyButtonConfig} disableNext= {fakeFunc}  enableNext={fakeFunc} transitionToNext= {fakeFunc} loggedIn={true} />       
        );

        let inputs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'input');
        expect(inputs[0].hasAttribute('checked'));
        TestUtils.Simulate.change(inputs[0], {target: {value: 'ethernet'}}); 
        expect(inputs[1].hasAttribute('checked'));
        sinon.assert.calledTwice(spyButtonConfig);

    }));

});