const RtAudio = require('bindings')('RtAudioJs');


console.log(RtAudio);
console.log(RtAudio.deviceProbe());

let streamParams = new RtAudio.RtStreamParams(3, 2, 0, 48000);
console.log(streamParams)
streamParams.sampleRate = 44100;
console.log(streamParams);

//Test Async Methods

RtAudio.factorize(9007199254740991, function(result) {
    console.log("From JS Callback");
    console.log("Factor: ", result);
}, function() {
    console.log("Facorization complete.");
});

console.log("This should output before the result log");