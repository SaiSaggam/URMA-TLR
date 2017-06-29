import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import CellularSimStatus from './cellular-sim-status.jsx';

describe('Cellular Connection Polling(Wizard) tests', function () {
    it('should render cellular results components', function () {
        let cellularIntState = {_id: 1,sim_status: 'Using SIM1', signal_strength: "Poor", signal_quality: 'Excellent','ip_address': '10.52.18.176',iccid: '', _groupIndex: 1} ;
        let root = TestUtils.renderIntoDocument(<CellularSimStatus cellularIntState={cellularIntState}/>);
        let labels = TestUtils.scryRenderedDOMComponentsWithTag(root, 'label');
        let spans = TestUtils.scryRenderedDOMComponentsWithTag(root, 'span');
        expect(labels.length).to.equal(4);
        expect(labels[0].textContent).to.equal('SIM Status:');
        expect(labels[1].textContent).to.equal('ICCID:');
        expect(labels[2].textContent).to.equal('Signal Strength:');
        expect(labels[3].textContent).to.equal('Signal Quality:');
        expect(spans[0].textContent).to.equal('Using SIM1');
        expect(spans[1].textContent).to.equal('');
        expect(spans[2].textContent).to.equal('Poor');
        expect(spans[3].textContent).to.equal('Excellent');
    });
    
    it('when cellular state has some values missing should display empty for those', function () {
        let cellularIntState = {_id: 1,sim_status: 'Using SIM1','ip_address': '10.52.18.176'} ;
        let root = TestUtils.renderIntoDocument(<CellularSimStatus cellularIntState={cellularIntState}/>);
        let labels = TestUtils.scryRenderedDOMComponentsWithTag(root, 'label');
        let spans = TestUtils.scryRenderedDOMComponentsWithTag(root, 'span');
        expect(labels.length).to.equal(4);
        expect(labels[0].textContent).to.equal('SIM Status:');
        expect(labels[1].textContent).to.equal('ICCID:');
        expect(labels[2].textContent).to.equal('Signal Strength:');
        expect(labels[3].textContent).to.equal('Signal Quality:');
        expect(spans[0].textContent).to.equal('Using SIM1');
        expect(spans[1].textContent).to.equal('');
        expect(spans[2].textContent).to.equal('');
        expect(spans[3].textContent).to.equal('');
    });
});


