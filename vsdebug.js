// const RtAudio = require('bindings')('RtAudioJs');


const EventEmitter = require('events');
var make_factorizer = function (n) {
    const worker = require('bindings')('RtAudioJs');
    let emitter = new EventEmitter();
    worker.factorize(n,
        function (event, value) {
            emitter.emit(event, value);
        },
        function () {
            emitter.emit("close");
        },
        function (error) {
            emitter.emit("error", error);
        });
    return emitter;
}

var addon = make_factorizer(9007199254740991);

addon.on('factor', function (factor) {
    console.log("Factor: " + factor);
});
addon.on('close', function () {
    console.log("Factorization is complete");
})
addon.on('error', function (e) {
    console.log(e);
});

// console.log(RtAudio);
// console.log(RtAudio.deviceProbe());

// let streamParams = new RtAudio.RtStreamParams(3, 2, 0, 48000);
// console.log(streamParams)
// streamParams.sampleRate = 44100;
// console.log(streamParams);

// //Test Async Methods

// RtAudio.factorize(9007199254740991, function(result) {
//     console.log("From JS Callback");
//     console.log("Factor: ", result);
// }, function() {
//     console.log("Facorization complete.");
// });

// console.log("This should output before the result log");