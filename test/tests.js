

const         test      = require('tap').test
            , dsound  = require('bindings')('RTAJSDS')
            , wasapi  = require('bindings')('RTAJSWASAPI')
            , dsfixture   = require('./fixtures/dsound.json')
            , wfixture   = require('./fixtures/wasapi.json')
            ;

test('deviceProbe DirectSound', function(t) {
    t.plan(2); // Tell tap there will be 2 test(s) in this suite
    var result;
    t.ok(result = dsound.deviceProbe());
    t.same(result, dsfixture);

    
});
test('deviceProbe WASAPI', function(t) {
    t.plan(2); // Tell tap there will be 2 test(s) in this suite
    var result;
    t.ok(result = wasapi.deviceProbe());
    t.same(result, wfixture);
});