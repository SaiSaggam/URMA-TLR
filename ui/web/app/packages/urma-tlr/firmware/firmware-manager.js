import {EventEmitter} from 'events';
import { FW_STATUS, FW_EVENT } from './firmware-constants';

/**
 * This class manages the firmware update process. It is a subclass of EventEmitter and emits the following events:
 * - FW_EVENT.START: action event to start the firmware update. Argument (e) is the event that initiated the call
 *                    (usually a click event)
 * - FW_EVENT.CANCEL: action event to cancel the firmware update. Argument (e) is the event that initiated the call
 *                    (usually a click event)
 * - FW_EVENT.PROGRESS: progress update. The progress argument is integer represending % complete. The message
 *                       argument is an optional message to be displayed with the progress.
 * - FW_EVENT.SUCCESS: firmware update completed successfully. The message argument is an option message to display.
 * - FW_EVENT.ERROR: firmware update failed. The err argument is a Javascript Error object that can be thrown.
 * - FW_EVENT.ABORTED: firmware update aborted. No arguments.
 */
export class FirmwareManager extends EventEmitter {

    /**
     * Start the firmware update. Emits FW_EVENT.START.
     * @param e the html event (usually onClick event).
     */
    startUpdate(e) {
        this.emit(FW_EVENT.START, e);
    }

    /**
     * Cancel the firmware update. Emits FW_EVENT.CANCEL.
     * @param e the html event (usually onClick event).
     */
    cancelUpdate(e) {
        this.emit(FW_EVENT.CANCEL, e);
    }

    /**
     * Update progress of the firmware update. Emits FW_EVENT.PROGRESS.
     * @param progress progress (0-100 percent)
     * @param message optional message to display
     */
    onProgress(progress, message) {
        if (message) {
            this.emit(FW_EVENT.PROGRESS, progress, message);
        } else {
            this.emit(FW_EVENT.PROGRESS, progress);
        }
    }

    /**
     * Notification of successful firmware update. Emits FW_EVENT.SUCCESS.
     * @param message optional message to display
     */
    onSuccess(message) {
        if (message) {
            this.emit(FW_EVENT.SUCCESS, message);
        } else {
            this.emit(FW_EVENT.SUCCESS);
        }
    }

    /**
     * Notification of failed firmware update. Emits FW_EVENT.ERROR.
     * @param err javascript Error object
     */
    onError(err) {
        this.emit(FW_EVENT.ERROR, err);
    }

    /**
     * Notification of aborted firmware update. Emits FW_EVENT.ABORTED.
     */
    onAborted() {
        this.emit(FW_EVENT.ABORTED);
    }
}
