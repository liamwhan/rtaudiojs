



const assert = require('chai').assert
    , deviceInfo = require(__dirname + '/fixtures/dsDeviceInfo.json')
    , streamParamsDefault = { deviceId: 0, nChannels: 2, firstChannel: 0, sampleRate: 48000 }
    ;

describe('RtAudioJs', function () {
    describe('deviceProbe()', function () {
        it("should return a deviceInfo object that matches the fixture", function () {
            let RtAudioJs = require('../build/Release/RtAudioJs');
            let deviceInfoResult = RtAudioJs.deviceProbe();
            assert.deepEqual(deviceInfoResult, deviceInfo);
        });

    });

    // describe('RtAudioJs::RtStreamParams', function () {

    //     describe('#constructor()', function () {
    //         // Test Construction of 
    //         let streamParamsResult
    //             , streamParamsOnly1st = { deviceId: 0 }
    //             , streamParams1and2 = { deviceId: 0, nChannels: 2 }
    //             , streamParams12and3 = { deviceId: 0, nChannels: 2, firstChannel: 0 }
    //             , streamParamsNo1FAIL = { nChannels: 2, firstChannel: 0, sampleRate: 48000 }
    //             , deviceId = 0
    //             , nChannels = 2
    //             , firstChannel = 0
    //             , sampleRate = 48000
    //             ;
    //         it("Should construct successfully is all params passed as object or separately", function () {
    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(streamParamsDefault)
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);
    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(deviceId, nChannels, sampleRate, firstChannel);
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);
    //         });

    //         it("Should construct successfully if only 1st argument (deviceId) is defined", function () {
    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(streamParamsOnly1st)
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);
    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(deviceId);
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);
    //         });



    //         it("Should construct successfully if only 1st 2 arguments (deviceId, nChannels) are defined", function () {

    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(streamParams1and2);
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);
    //             assert.doesNotThrow(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams(deviceId, nChannels);
    //             });
    //             assert.deepEqual(streamParamsResult, streamParamsDefault);

    //         });

    //         it("Should throw SyntaxError if `new` not used", function () {

    //             assert.throws(function () {
    //                 streamParamsResult = RtAudioJs.RtStreamParams(streamParamsDefault);
    //             }, new SyntaxError("RtStreamParams must be instantiated.\nThis class can only be used with the `new` keyword e.g.\n`let streamParams = new RtAudioJs.RtStreamParams(0, 2, 0, 48000);`"));

    //         });
    //         it("Should throw TypeError if no arguments passed.", function () {

    //             assert.throws(function () {
    //                 streamParamsResult = new RtAudioJs.RtStreamParams();
    //             }, new TypeError("The \"deviceId\" argument is required and must be integer value"));

    //         });

    //     });

    // });
});




