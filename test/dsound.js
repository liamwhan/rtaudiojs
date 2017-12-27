

const         test      = require('tap').test
            , bindings  = require('bindings')('RtAudio')
            , fixture   = require('./fixtures/dsound.json')
            ;

test('deviceProbe', function(t) {
    t.plan(2); // Tell tap there will be 2 test(s) in this suite
    var result;
    t.ok(result = bindings.deviceProbe());
    t.equal(result, fixture);

    
});