

const         test      = require('tap').test
            , dsound  = require('bindings')('RtAudioJs')
            , wasapi  = require('bindings')('RTAJSWASAPI')
            , dsDeviceInfo   = require('./fixtures/dsDeviceInfo.json')
            , wDeviceInfo   = require('./fixtures/wasapi.json')
            , streamParams = { deviceId: 0, nChannels: 2, firstChannel: 0, sampleRate: 48000}
            ;

test('deviceProbe DirectSound', function(t) {
    // t.plan(2);
    let deviceInfoResult;
    t.ok(deviceInfoResult = dsound.deviceProbe());
    t.same(deviceInfoResult, dsDeviceInfo);
});


test('deviceProbe WASAPI', function(t) {
    // t.plan(2);
    let deviceInfoResult;
    t.ok(deviceInfoResult = wasapi.deviceProbe());
    t.same(deviceInfoResult, wDeviceInfo);
});


test('Get/Set Stream Params DirectSound', function(t) {
    
    // t.plan(3);
    let getStreamParamsResult;
    t.ok(dsound.setStreamParams(streamParams));
    t.ok(getStreamParamsResult = dsound.getStreamParams());
    t.same(getStreamParamsResult, streamParams);
    
}); 

test('Get/Set Stream Params WASAPI', function(t) {
    
    // t.plan(3);
    let getStreamParamsResult;
    t.ok(wasapi.setStreamParams(streamParams));
    t.ok(getStreamParamsResult = wasapi.getStreamParams());
    t.same(getStreamParamsResult, streamParams);

})