import { FW_STATUS } from './firmware-constants';

let log = new Logger('progress-monitor');

// Multipliers for calculating percentage progress done for steps in the firmware update process.
let DOWNLOADING_MULTIPLIER = .7;
let VERIFYING_MULTIPLIER = .05;
let APPLYING_MULTIPLIER = .20;

/**
 * Class that monitors progress of a firmware update. Calculates the overall progress based on progress
 * in each stage of the process. Emits events via the FirmwareManager.
 */
export default class ProgressMonitor {
    constructor(firmwareManager) {
        this.firmwareManager = firmwareManager;
        this.reset();
    }

    update(status, progress, result) {
        log.debug('progress update - status: '+status+', progress: '+progress+', result: '+result);

        let msg = null;
        switch(status) {
            case FW_STATUS.DOWNLOADING:
                this.downloading = Math.round(progress * DOWNLOADING_MULTIPLIER);
                msg = (progress < 100 ? 'Downloading firmware file.' : 'Firmware file downloaded.');
                this.firmwareManager.onProgress(this.getProgress(), msg);
                break;
            case FW_STATUS.VERIFYING:
                this.verifying = Math.round(progress * VERIFYING_MULTIPLIER);
                msg = (progress < 100 ? 'Verifying firmware image.': 'Firmware image verified.');
                this.firmwareManager.onProgress(this.getProgress(), msg);
                break;
            case FW_STATUS.APPLYING:
                this.applying = Math.round(progress * APPLYING_MULTIPLIER);
                msg = (progress < 100 ? 'Applying new firmware, please wait.' : 'New firmware applied.');
                this.firmwareManager.onProgress(this.getProgress(), msg);
                break;
            case FW_STATUS.COMPLETE:
                if(!this.done) {
                    this.done = true;
                    this.firmwareManager.onSuccess('Firmware update complete.');
                }
                break;
            case FW_STATUS.ERROR:
                this.done = true;
                msg = result || 'Unknown error';
                this.firmwareManager.onError(new Error('Firmware update failed: '+msg));
                break;
        }
        return this.getProgress();
    }

    getProgress() {
        return this.done ? 100 : (this.downloading + this.verifying + this.applying);
    }

    reset() {
        this.downloading = 0;
        this.verifying = 0;
        this.applying = 0;
        this.done = false;
    }
}
