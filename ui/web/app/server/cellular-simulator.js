import {UrmaServer,Collections} from 'meteor/digi:urma-core';

let log = new Logger('cellular-simulation');
let State = Collections.State;

/**
 * This class represents a step in the simulated cellular connection process. It creates a timer and simulates
 * progress within this step, updating cellular state in the State collection as it goes.. It also holds
 * the next step in the process and automatically transitions to the next step when this one is complete.
 */
class ccStep {
    constructor(status, delay, progressIncrement,value) {
        this.state = status;
        this.stateValue = value;
        this.delay = delay;
        this.progressIncrement = progressIncrement;
        this.nextStep = null;
        this.prevStep = null;
        this.progress = 0;
        this.interval = null;
    }

    /** Start the simulation for this step in the process. */
    start() {
        this.progress = 0;
        this.interval = Meteor.setInterval(this._progressUpdate.bind(this), this.delay);
    }

    /** Configure the next step in the process. Returns next step so that configuration can be chained. */
    next(simStep) {
        this.nextStep = simStep;
        simStep.prevStep = this;
        return this.nextStep;
    }

    /** Get the first step in the chain. Can start from any step in the chain. */
    first() {
        let first = this;
        while (first.prevStep) {
            first = first.prevStep;
        }
        return first;
    }

    /** Get the last step in the chain. Can start from any step in the chain. */
    last() {
        // Start with this step and walk the chain until we get to the last step.
        let last = this;
        while (last.nextStep) {
            last = last.nextStep;
        }
        return last;
    }

    /** progress updates when interval timer fires. */
    _progressUpdate() {
        this.progress += this.progressIncrement;

        if (this.progress >= 100) {
            ccSimulator.updateCellularState(this.state, this.stateValue);
            // done with this step.
            Meteor.clearInterval(this.interval);
            this._goNextStep();
        }
    }

    /** Move on to the next step in the process if there is one. */
    _goNextStep() {
        if (this.nextStep) {
            this.nextStep.start();
        }
    }
};

class CellularConnectionSimulator {

    reset() {
        let selector = {
            _groupName: 'cellular',
            _groupIndex: 0
        };
        let modifier = {
            $set: {
                sim_status: '',
                signal_strength: '',
                signal_quality: '',
                ip_address: ''
            }
        };
        State.upsert(selector, modifier);
    }

    /** Start the cellular connection simulation process. */
    simulateCellularConnection() {
        this.reset();
        let steps;
        steps = new ccStep('sim_status', 1000, 100,'Using SIM1')
            .next(new ccStep('signal_strength', 1000, 100,'Poor (-105dBm)'))
            .next(new ccStep('signal_quality', 1000, 100,'Excellent (-6dB)'))
            .next(new ccStep('ip_address', 1000, 100,'10.52.18.176'));

        // Start the simulation.
        log.debug('Starting cellular connection simulation.');
        steps.first().start();
    }

    /** Update cellular state in State collection based on the step. */
    updateCellularState(state, value) {
        let selector = {
            _groupName: 'cellular',
            _groupIndex: 0
        };

        let modifier = {
            $set: {
                [state]: value
            }
        };
        State.upsert(selector, modifier);
        log.debug('updating cellular state settings   ' + state + ':' + value);
    }
};

const ccSimulator = new CellularConnectionSimulator();
export default ccSimulator;