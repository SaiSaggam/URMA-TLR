import {Collections} from 'meteor/digi:urma-core';
import {FW_STATUS} from 'meteor/digi:urma-tlr';

let log = new Logger('fwupdate-simulation');

let State = Collections.State;

/**
 * This class represents a step in the simulated firmware update process. It creates a timer and simulates
 * progress within this step, updating firmware_status in the State collection as it goes.. It also holds
 * the next step in the process and automatically transitions to the next step when this one is complete.
 */
class SimStep {
    constructor(status, delay, progressIncrement) {
        this.status = status;
        this.delay = delay;
        this.progressIncrement = progressIncrement;
        this.nextStep = null;
        this.prevStep = null;
        this.progress = 0;
        this.interval = null;
    }

    /** Start the simulation for this step in the process. */
    start() {
        // First update status with start of this step.
        this.progress = 0;
        fupSimulator.updateFirmwareStatus(this.status, this.progress);
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
        fupSimulator.updateFirmwareStatus(this.status, this.progress);

        if (this.progress >= 100) {
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

class FirmwareUpdateSimulator {

    reset() {
        this.updateFirmwareStatus(FW_STATUS.NONE, 0);
    }

    /** Start the firmware update simulation process. */
    simulateFirmwareUpdate(filename, options) {
        this.reset();

        let steps;
        if (options.remoteLoad) {
            // Auto update. Start with download step (device downloads file from firmware repository).
            steps = new SimStep(FW_STATUS.DOWNLOADING, 100, 10)
                .next(new SimStep(FW_STATUS.VERIFYING, 100, 100))
                .next(new SimStep(FW_STATUS.APPLYING, 100, 10))
                .next(new SimStep(FW_STATUS.COMPLETE, 100, 100));
        } else {
            // Manual update. File already downloaded via HTTP. Start with verification step.
            steps = new SimStep(FW_STATUS.VERIFYING, 1000, 100)
                .next(new SimStep(FW_STATUS.APPLYING, 1000, 10))
                .next(new SimStep(FW_STATUS.COMPLETE, 1000, 100));
        }

        if (options.autoReboot) {
            steps.last().next(new SimStep(FW_STATUS.REBOOTING, 30000, 100));
        }

        // Start the simulation.
        log.debug('Starting firmware update simulation. file:', filename, ' options:', options);
        steps.first().start();
    }

    /** Update firmware_status in State collection. */
    updateFirmwareStatus(status, progress) {
        log.debug('firmware update... status:', status, ', progress:', progress);
        let selector = {
            _groupName: 'firmware_status',
            _groupIndex: 0
        };
        let modifier = {
            $set: {
                status: status,
                progress: progress
            }
        };
        State.upsert(selector, modifier);
    }

};

const fupSimulator = new FirmwareUpdateSimulator();
export default fupSimulator;
