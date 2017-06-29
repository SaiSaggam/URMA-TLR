import Urma from 'meteor/digi:urma-core';

let alerts = Urma.alerts;

describe('Alerts', function () {
    let stubFunction = function() {};

    it('should use default options when none provided', sinon.test(function () {
        let stubInfo = this.stub(toastr, "info", stubFunction);
        let stubSuccess = this.stub(toastr, "success", stubFunction);
        let stubWarning = this.stub(toastr, "warning", stubFunction);
        let stubError = this.stub(toastr, "error", stubFunction);

        alerts.info('My info message', 'Just FYI');
        alerts.success('My success message', 'Congratulations');
        alerts.warning('My warning message', 'Oops');
        alerts.error('My error message', 'You Have Failed');

        sinon.assert.calledWith(stubInfo, 'My info message', 'Just FYI', undefined);
        sinon.assert.calledWith(stubSuccess, 'My success message', 'Congratulations', undefined);
        sinon.assert.calledWith(stubWarning, 'My warning message', 'Oops', undefined);
        sinon.assert.calledWith(stubError, 'My error message', 'You Have Failed', undefined);
    }));

    it('should override default options with options argument', sinon.test(function () {
        let stubInfo = this.stub(toastr, "info", stubFunction);
        let stubSuccess = this.stub(toastr, "success", stubFunction);
        let stubWarning = this.stub(toastr, "warning", stubFunction);
        let stubError = this.stub(toastr, "error", stubFunction);

        // Override default timeout and showMethod options
        let overrideOptions = {timeout: 8000, showMethod: 'fadeIn'};
        alerts.info('My info message', 'Just FYI', overrideOptions);
        alerts.success('My success message', 'Congratulations', overrideOptions);
        alerts.warning('My warning message', 'Oops', overrideOptions);
        alerts.error('My error message', 'You Have Failed', overrideOptions);

        sinon.assert.calledWith(stubInfo, 'My info message', 'Just FYI', overrideOptions);
        sinon.assert.calledWith(stubSuccess, 'My success message', 'Congratulations', overrideOptions);
        sinon.assert.calledWith(stubWarning, 'My warning message', 'Oops', overrideOptions);
        sinon.assert.calledWith(stubError, 'My error message', 'You Have Failed', overrideOptions);
    }));

    it('should handle undefined message and title', sinon.test(function () {
        let stubInfo = this.stub(toastr, "info", stubFunction);
        let stubSuccess = this.stub(toastr, "success", stubFunction);
        let stubWarning = this.stub(toastr, "warning", stubFunction);
        let stubError = this.stub(toastr, "error", stubFunction);

        alerts.info();
        alerts.success();
        alerts.warning();
        alerts.error();

        sinon.assert.calledWith(stubInfo, 'Unknown', undefined, undefined);
        sinon.assert.calledWith(stubSuccess, 'Unknown', undefined, undefined);
        sinon.assert.calledWith(stubWarning, 'Unknown', undefined, undefined);
        sinon.assert.calledWith(stubError, 'Unknown', undefined, undefined);
    }));

    it('should allow clearing alerts', sinon.test(function () {
        let stubInfo = this.stub(toastr, "info", stubFunction);
        let stubClear = this.stub(toastr, "clear", stubFunction);

        let lastAlert = alerts.info('My info message', 'Just FYI');

        // Clear specific alert
        alerts.clear(lastAlert);
        sinon.assert.calledWith(stubClear, lastAlert);

        // Clear all alerts
        alerts.clear();
        sinon.assert.calledWith(stubClear, undefined);
    }));
});
