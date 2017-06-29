import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import CountdownTimer from './countdown-timer.jsx';

describe('Countdown Timer Tests', function() {
    
    it('should format time remaining', sinon.test(function () {
        let timer = TestUtils.renderIntoDocument(<CountdownTimer timeRemaining={3000}/>);
        let timeRemaining = TestUtils.findRenderedDOMComponentWithClass(timer, 'countdown-timer-remaining');

        expect(timeRemaining.textContent).to.equal('00:03');

        let timer2 = TestUtils.renderIntoDocument(<CountdownTimer timeRemaining={150000}/>);
        let timeRemaining2 = TestUtils.findRenderedDOMComponentWithClass(timer2, 'countdown-timer-remaining');
        expect(timeRemaining2.textContent).to.equal('02:30');
    }));

});