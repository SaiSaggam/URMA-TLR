import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let Authorized = Urma.Authorized;

describe('Authorized Component Tests', function() {

    it('should render content when logged in', sinon.test(function () {
        let stubUser = this.stub(Meteor, "user", ()=> {return {username: 'user'}});
        let root = TestUtils.renderIntoDocument(
            <Authorized>
                <div>
                    <p>Authorized content</p>
                </div>
            </Authorized>
        );

        sinon.assert.calledOnce(stubUser);

        // Logged in. Should render all content (wrapped in a div).
        let div = TestUtils.scryRenderedDOMComponentsWithTag(root, 'div');
        let p = TestUtils.findRenderedDOMComponentWithTag(root, 'p');

        expect(div.length).to.equal(2);
        expect(p.textContent).to.equal('Authorized content');
    }));

    it('should not render content when not logged in', sinon.test(function() {
        let stubUser = this.stub(Meteor, "user", ()=> {return null});
        let root = TestUtils.renderIntoDocument(
            <Authorized>
                <div>
                    <p>Authorized content</p>
                </div>
            </Authorized>
        );

        sinon.assert.calledOnce(stubUser);

        // Logged out. Should render empty div.
        let div = TestUtils.scryRenderedDOMComponentsWithTag(root, 'div');
        let p = TestUtils.scryRenderedDOMComponentsWithTag(root, 'p');

        expect(div.length).to.equal(1);
        expect(p.length).to.equal(0);
    }));
});
