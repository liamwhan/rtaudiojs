

const test = require('tap').test
    , dsound = require('bindings')('RtAudioJs')
    , dsDeviceInfo = require('./fixtures/dsDeviceInfo.json')
    , streamParamsDefault = { deviceId: 0, nChannels: 2, firstChannel: 0, sampleRate: 48000 }
    ;

test('deviceProbe', function (t) {
    t.plan(2);
    let deviceInfoResult;
    t.ok(deviceInfoResult = dsound.deviceProbe());
    t.same(deviceInfoResult, dsDeviceInfo);
});


test('RtStreamParams Constructor tests', function (t) {

    t.plan(14);

    // Test Construction of 
    let streamParamsResult
        , streamParamsOnly1st = { deviceId: 0 }
        , streamParams1and2 = { deviceId: 0, nChannels: 2 }
        , streamParams12and3 = { deviceId: 0, nChannels: 2, firstChannel: 0 }
        , streamParamsNo1FAIL = { nChannels: 2, firstChannel: 0, sampleRate: 48000 }
        , deviceId = 0
        , nChannels = 2
        , firstChannel = 0
        , sampleRate = 48000
        ;

    // Should construct successfully is all params passed as object or separately
    t.doesNotThrow(function () {
        streamParamsResult = new dsound.RtStreamParams(streamParamsDefault)
    });
    t.same(streamParamsResult, streamParamsDefault);
    t.doesNotThrow(function () {
        streamParamsResult = new dsound.RtStreamParams(deviceId, nChannels, sampleRate, firstChannel);
    });
    t.same(streamParamsResult, streamParamsDefault);

    // Should construct successfully if only 1st argument (deviceId) is defined
    t.doesNotThrow(function () {
        streamParamsResult = new dsound.RtStreamParams(streamParamsOnly1st)
    });
    t.same(streamParamsResult, streamParamsDefault);
    t.doesNotThrow(function () {
        streamParamsResult = new dsound.RtStreamParams(deviceId);
    });
    t.same(streamParamsResult, streamParamsDefault);

    // Should construct successfully if only 1st 2 arguments (deviceId, nChannels) are defined
    t.doesNotThrow(function () {
        streamParamsResult = new dsound.RtStreamParams(streamParams1and2);
    });
    t.same(streamParamsResult, streamParamsDefault);
    t.doesNotThrow(function(){
        streamParamsResult = new dsound.RtStreamParams(deviceId, nChannels);
    });
    t.same(streamParamsResult, streamParamsDefault);

    //Throws SyntaxError if `new` not used
    t.throws(function () {
        streamParamsResult = dsound.RtStreamParams(streamParamsDefault);
    }, new SyntaxError("RtStreamParams must be instantiated.\nThis class can only be used with the `new` keyword e.g.\n`let streamParams = new RtAudioJs.RtStreamParams(0, 2, 0, 48000);`"));

    //Throws TypeError if no arguments passed.
    t.throws(function () {
        streamParamsResult = new dsound.RtStreamParams();
    }, new TypeError("The \"deviceId\" argument is required and must be integer value"))

});