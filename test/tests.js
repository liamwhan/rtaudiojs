

const         test      = require('tap').test
            , dsound  = require('bindings')('RtAudioJs')
            , wasapi  = require('bindings')('RTAJSWASAPI')
            , dsDeviceInfo   = require('./fixtures/dsDeviceInfo.json')
            , wDeviceInfo   = require('./fixtures/wasapi.json')
            , streamParams = { deviceId: 0, nChannels: 2, firstChannel: 0, sampleRate: 48000}
            , streamParams2 = { deviceId: 0, nChannels: 2, sampleRate: 48000} // no firstChanell - to test if default values works
            ;

test('deviceProbe DirectSound', function(t) {
    t.plan(2);
    let deviceInfoResult;
    t.ok(deviceInfoResult = dsound.deviceProbe());
    t.same(deviceInfoResult, dsDeviceInfo);
});


test('deviceProbe WASAPI', function(t) {
    t.plan(2);
    let deviceInfoResult;
    t.ok(deviceInfoResult = wasapi.deviceProbe());
    t.same(deviceInfoResult, wDeviceInfo);
});


test('Get/Set Stream Params DirectSound', function(t) {
    
    t.plan(6);

    // with explicit firstchannel
    let getStreamParamsResult;
    t.ok(dsound.setStreamParams(streamParams));
    t.ok(getStreamParamsResult = dsound.getStreamParams());
    t.same(getStreamParamsResult, streamParams);

    // with default firstChannel
    t.ok(dsound.setStreamParams(streamParams2));
    t.ok(getStreamParamsResult = dsound.getStreamParams());
    t.same(getStreamParamsResult, streamParams); //Should still have firstChannel set, so we compare with original streamParams
    
}); 

test('Get/Set Stream Params WASAPI', function(t) {
    
    t.plan(6);

    let getStreamParamsResult;
    t.ok(wasapi.setStreamParams(streamParams));
    t.ok(getStreamParamsResult = wasapi.getStreamParams());
    t.same(getStreamParamsResult, streamParams);

    t.ok(wasapi.setStreamParams(streamParams2));
    t.ok(getStreamParamsResult = wasapi.getStreamParams());
    t.same(getStreamParamsResult, streamParams); //Should still have firstChannel set, so we compare with original streamParams

})