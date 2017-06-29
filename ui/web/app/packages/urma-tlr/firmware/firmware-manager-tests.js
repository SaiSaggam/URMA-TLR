import {FirmwareManager} from './firmware-manager';
import { FW_STATUS, FW_EVENT } from './firmware-constants';

describe('FirmwareManager', function () {
    let fwMgr = new FirmwareManager();
    let listenerStub;

    beforeEach(()=> {
        listenerStub = {
            startUpdate: sinon.spy(),
            cancelUpdate: sinon.spy(),
            onProgress: sinon.spy(),
            onSuccess: sinon.spy(),
            onError: sinon.spy(),
            onAborted: sinon.spy()
        };

        fwMgr
            .on(FW_EVENT.START, listenerStub.startUpdate)
            .on(FW_EVENT.CANCEL, listenerStub.cancelUpdate)
            .on(FW_EVENT.PROGRESS, listenerStub.onProgress)
            .on(FW_EVENT.SUCCESS, listenerStub.onSuccess)
            .on(FW_EVENT.ERROR, listenerStub.onError)
            .on(FW_EVENT.ABORTED, listenerStub.onAborted);

    });

    afterEach(()=> {
        fwMgr.removeAllListeners();
    })

    it('should emit start event', sinon.test(function () {
        let e = {bogus: 'event'};
        fwMgr.startUpdate(e);
        sinon.assert.calledOnce(listenerStub.startUpdate);
        sinon.assert.calledWithExactly(listenerStub.startUpdate, e);
    }));

    it('should emit cancel event', sinon.test(function () {
        let e = {bogus: 'event'};
        fwMgr.cancelUpdate(e);
        sinon.assert.calledOnce(listenerStub.cancelUpdate);
        sinon.assert.calledWithExactly(listenerStub.cancelUpdate, e);
    }));

    it('should emit progress event', sinon.test(function () {
        let progress = 27;
        fwMgr.onProgress(progress);
        sinon.assert.calledOnce(listenerStub.onProgress);
        sinon.assert.calledWithExactly(listenerStub.onProgress, progress);
    }));

    it('should emit progress event with message', sinon.test(function () {
        let progress = 32;
        let message = 'progress has been made.'
        fwMgr.onProgress(progress, message);
        sinon.assert.calledOnce(listenerStub.onProgress);
        sinon.assert.calledWithExactly(listenerStub.onProgress, progress, message);
    }));

    it('should emit success event', sinon.test(function () {
        fwMgr.onSuccess();
        sinon.assert.calledOnce(listenerStub.onSuccess);
        sinon.assert.calledWithExactly(listenerStub.onSuccess);
    }));

    it('should emit success event with message', sinon.test(function () {
        let message = 'success!';
        fwMgr.onSuccess(message);
        sinon.assert.calledOnce(listenerStub.onSuccess);
        sinon.assert.calledWithExactly(listenerStub.onSuccess, message);
    }));

    it('should emit error event', sinon.test(function () {
        let err = new Error('this is very bad news!');
        fwMgr.onError(err);
        sinon.assert.calledOnce(listenerStub.onError);
        sinon.assert.calledWithExactly(listenerStub.onError, sinon.match.instanceOf(Error));
    }));

    it('should emit aborted event', sinon.test(function () {
        fwMgr.onAborted();
        sinon.assert.calledOnce(listenerStub.onAborted);
        sinon.assert.calledWithExactly(listenerStub.onAborted);
    }));

});
