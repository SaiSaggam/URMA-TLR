import React from 'react';
import TestUtils from 'react-addons-test-utils';
import {expect} from 'chai';
import FirmwareSelect from './firmware-select.jsx';

describe('Firmware Select', function() {
    let fwFiles, fwSelect, autoOption;
    let spyHandleSelect = sinon.spy();
    let spySetInitialState = sinon.spy();

    it('should find most recent firmware version', sinon.test(function() {

        fwFiles = [
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.0.0.0'
            },
            {
                name: 'lr54-0.1.0.1.bin',
                size: 23456780,
                date: '3/14/2016 9:45AM',
                version: '0.1.0.1'
            },
            {
                name: 'lr54-0.2.0.2.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '0.2.0.2'
            }
        ];


        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={false}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '0.1.0.0'
            />
        );

        autoOption = TestUtils.findRenderedDOMComponentWithClass(fwSelect, 'firmware-update-option-auto');

        sinon.assert.calledWith(spySetInitialState, fwFiles[0], false);
        expect(autoOption.textContent).to.equal('2.0.0.0');

    }));
    it('should sort by minor release', sinon.test(function() {

        fwFiles = [
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.0.0.0'
            },
            {
                name: 'lr54-2.1.0.1.bin',
                size: 23456780,
                date: '3/14/2016 9:45AM',
                version: '2.1.0.1'
            },
            {
                name: 'lr54-2.0.0.2.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.0.0.2'
            }
        ];

        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={false}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '0.1.0.0'
            />
        );

        autoOption = TestUtils.findRenderedDOMComponentWithClass(fwSelect, 'firmware-update-option-auto');

        sinon.assert.calledWith(spySetInitialState, fwFiles[1], false);
        expect(autoOption.textContent).to.equal('2.1.0.1');

    }));

    it ('should sort by patch', sinon.test(function() {
        fwFiles = [
            {
                name: 'lr54-2.1.0.1.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.1.0.1'
            },
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23456780,
                date: '3/14/2016 9:45AM',
                version: '2.0.0.0'
            },
            {
                name: 'lr54-0.2.0.2.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.1.0.2'
            }
        ];

        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={false}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '0.1.0.0'
            />
        );

        autoOption = TestUtils.findRenderedDOMComponentWithClass(fwSelect, 'firmware-update-option-auto');

        sinon.assert.calledWith(spySetInitialState, fwFiles[2], false);
        expect(autoOption.textContent).to.equal('2.1.0.2');

    }));

    it('should show firmware version if only one version in list', sinon.test(function() {

        fwFiles = [
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.0.0.0'
            }
        ];

        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={false}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '0.1.0.0'
            />
        );

        autoOption = TestUtils.findRenderedDOMComponentWithClass(fwSelect, 'firmware-update-option-auto');

        sinon.assert.calledWith(spySetInitialState, fwFiles[0], false);
        expect(autoOption.textContent).to.equal('2.0.0.0');
    }));

    it('should display message and hide select form if firmware up to date', sinon.test(function() {

        fwFiles = [
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.0.0.0'
            }
        ];

        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={false}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '2.0.0.0'
            />
        );

        let upToDateMessage = TestUtils.findRenderedDOMComponentWithClass(fwSelect, 'firmware-update-message');


        sinon.assert.calledWith(spySetInitialState, fwFiles[0], true);
        expect(upToDateMessage).to.not.be.undefined;
        expect(upToDateMessage.textContent).to.equal('The device\'s firmware is up to date');

    }));

    it('should display all firmware options if in development mode', sinon.test(function () {

        fwFiles = [
            {
                name: 'lr54-2.1.0.1.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.1.0.1'
            },
            {
                name: 'lr54-2.0.0.0.bin',
                size: 23456780,
                date: '3/14/2016 9:45AM',
                version: '2.0.0.0'
            },
            {
                name: 'lr54-0.2.0.2.bin',
                size: 23467890,
                date: '3/24/2016 11:12AM',
                version: '2.1.0.2'
            }
        ];

        fwSelect = TestUtils.renderIntoDocument(
            <FirmwareSelect
                developmentMode={true}
                setInitialFwState={spySetInitialState}
                handleSelect={spyHandleSelect}
                fwFiles={fwFiles}
                currentFwVersion = '0.1.0.0'
            />
        );

        let autoOptions = TestUtils.scryRenderedDOMComponentsWithClass(fwSelect, 'firmware-update-option-auto');
        expect(autoOptions.length).to.equal(3);
    }));
});