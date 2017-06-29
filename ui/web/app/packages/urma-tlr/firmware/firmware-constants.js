/** Status values for firmware update in the firmware_status RCI state group */
export const FW_STATUS = {
    NONE: 0,
    DOWNLOADING: 1,
    VERIFYING: 2,
    APPLYING: 3,
    COMPLETE: 4,
    REBOOTING: 5,
    ERROR: 6
};

/** Events emitted from FirmwareManager */
export const FW_EVENT = {
    START: 'start', // action event to start fw update
    CANCEL: 'cancel', // action event to cancel (abort) fw update
    PROGRESS: 'progress', // progress update
    SUCCESS: 'success', // fw completed successfully
    ERROR: 'error', // fw failed
    ABORTED: 'aborted' // fw was aborted
}
