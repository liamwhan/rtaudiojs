const RtAudio = require('bindings')('RtAudioJs');


console.log(RtAudio);
console.log(RtAudio.deviceProbe());

let streamParams = new RtAudio.RtStreamParams(3, 2, 0, 48000);
console.log(streamParams)
streamParams.sampleRate = 44100;
console.log(streamParams);

//Test Async Methods

RtAudio.primes(3000, function(result) {
    console.log("From JS Callback");
    console.log("result: ", result);
    // console.log(`Random Result Value at ${i}:`, result[i]);
});
// RtAudio.primes_progress(3000, function(result) {
//     console.log("From JS Callback");
//     console.log("result: ", result);
//     // console.log(`Random Result Value at ${i}:`, result[i]);
// }, function(progress) {
//     console.log("Progress: ", progress);
// });
console.log("This should output before the result log");