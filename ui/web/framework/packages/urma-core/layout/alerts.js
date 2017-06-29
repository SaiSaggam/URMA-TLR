const DFT_MESSAGE = 'Unknown';

/**
 * Shows popup alert messages.
 */
class Alerts {

    constructor() {

        this._defaultOptions = toastr.options = {
            closeButton: true,
            debug: false,
            newestOnTop: true,
            progressBar: true,
            positionClass: "toast-top-right",
            preventDuplicates: true,
            onclick: null,
            showDuration: 300,
            hideDuration: 1000,
            timeOut: 5000,
            extendedTimeOut: 1000,
            showEasing: "swing",
            hideEasing: "linear",
            showMethod: "slideDown",
            hideMethod: "slideUp"
        };
    }

    info(message, title, options) {
        return toastr.info(message || DFT_MESSAGE, title, options);
    }

    success(message, title, options) {
        return toastr.success(message || DFT_MESSAGE, title, options);
    }

    warning(message, title, options) {
        return toastr.warning(message || DFT_MESSAGE, title, options);
    }

    error(message, title, options) {
        return toastr.error(message || DFT_MESSAGE, title, options);
    }

    clear(toastrEl) {
        toastr.clear(toastrEl);
    }

}

// Create a singleton instance that will be exported
alerts = new Alerts();

export default alerts;