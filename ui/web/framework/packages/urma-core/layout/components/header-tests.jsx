import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import Urma from 'meteor/digi:urma-core';
import {UrmaTest} from 'meteor/digi:urma-core';

let Header = Urma.Header;
let LoginButtons = Urma.LoginButtons;

describe('Header Component Tests', function () {
    let fakeLoginService = { login: ()=>{}, logout: ()=>{} };

    it('should render all header components', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(
            <Header title="My Title" logo="/images/logo.png" loginService={fakeLoginService}/>
        );

        let h2 = TestUtils.findRenderedDOMComponentWithTag(root, 'h2');
        let img = TestUtils.findRenderedDOMComponentWithTag(root, 'img');
        let loginButtons = TestUtils.findRenderedComponentWithType(root, LoginButtons);

        expect(h2.textContent).to.equal('My Title');
        expect(img.getAttribute('src')).to.equal('/images/logo.png');
        expect(loginButtons).to.exist;
        expect(loginButtons.props.loginService).to.equal(fakeLoginService);
    }));

    it('should render without logo image', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<Header title="My App" loginService={fakeLoginService} />);

        let h2 = TestUtils.findRenderedDOMComponentWithTag(root, 'h2');
        let imgs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'img');
        let loginButtons = TestUtils.findRenderedComponentWithType(root, LoginButtons);

        expect(h2.textContent).to.equal('My App');
        expect(imgs.length).to.equal(0);
        expect(loginButtons).to.exist;
    }));

    it('should not render login buttons when no login service provided', sinon.test(function () {
        let root = TestUtils.renderIntoDocument(<Header title="My App" />);

        let h2 = TestUtils.findRenderedDOMComponentWithTag(root, 'h2');
        let imgs = TestUtils.scryRenderedDOMComponentsWithTag(root, 'img');
        let loginButtons = TestUtils.scryRenderedComponentsWithType(root, LoginButtons);

        expect(h2.textContent).to.equal('My App');
        expect(imgs.length).to.equal(0);
        expect(loginButtons.length).to.equal(0);
    }));
});
