import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Welcome from './welcome.jsx';

let fakeFunc = function () {};

describe('Welcome (Wizard) tests', function () {

    it('should render welcome components', function () {
        let root = TestUtils.renderIntoDocument(
            <Welcome buttonConfig={fakeFunc} disableNext= {fakeFunc}  enableNext={fakeFunc} transitionToNext= {fakeFunc} loggedIn={true} />
        );
        let header = TestUtils.findRenderedDOMComponentWithTag(root, 'h1');
        
        expect(header.textContent).to.equal('Welcome to the Getting Started Wizard');
        
    });

    it('should call buttonConfig, enableNext and not call transitionToNext when welcome is rendered', sinon.test(function () {
        let spyTransition = this.spy();
        let spyButtonConfig = this.spy();
        let spyEnableNext = this.spy();
        let root = TestUtils.renderIntoDocument(
            <Welcome loggedIn={true} enableNext={spyEnableNext} buttonConfig={spyButtonConfig} transitionToNext={spyTransition}/>
        );

        sinon.assert.calledOnce(spyButtonConfig);
        sinon.assert.calledOnce(spyEnableNext);
        sinon.assert.notCalled(spyTransition);

    }));

    it ("should have links for quick start guide, online guide and verify the url value is not empty ", function() {
        let root = TestUtils.renderIntoDocument(
            <Welcome buttonConfig={fakeFunc} disableNext= {fakeFunc}  enableNext={fakeFunc} transitionToNext= {fakeFunc} loggedIn={true} />
        );
        let links = TestUtils.scryRenderedDOMComponentsWithTag(root, 'a');
        expect(links.length).to.equal(2);
        expect(links[0].textContent).to.equal('Quick Start Guide');
        expect(links[1].textContent).to.equal('Online User Manual');
        expect(links[0].getAttribute('href').length).not.equal(0);
        expect(links[1].getAttribute('href').length).not.equal(0);

    });

});