const RtAudio = require('./build/Debug/RtAudioJs');


console.log(RtAudio);
let streamParams = new RtAudio.RtStreamParams(3, 2, 0, 48000);
console.log(streamParams)
streamParams.sampleRate = 44100;
console.log(streamParams);

 // Should construct successfully is all params passed as object or separately
 streamParamsResult = new RtAudio.RtStreamParams(streamParamsDefault);
 t.same(streamParamsResult, streamParamsDefault);
 streamParamsResult = new RtAudio.RtStreamParams(deviceId, nChannels,  sampleRate, firstChannel));
 t.same(streamParamsResult, streamParamsDefault);

 // Should construct successfully if only 1st argument (deviceId) is defined
 streamParamsResult = new RtAudio.RtStreamParams(streamParamsOnly1st));
 t.same(streamParamsResult, streamParamsDefault);
 streamParamsResult = new RtAudio.RtStreamParams(deviceId));
 t.same(streamParamsResult, streamParamsDefault);
 
 // Should construct successfully if only 1st 2 arguments (deviceId, nChannels) are defined
 streamParamsResult = new RtAudio.RtStreamParams(streamParams1and2));
 t.same(streamParamsResult, streamParamsDefault);
 streamParamsResult = new RtAudio.RtStreamParams(deviceId, nChannels));
 t.same(streamParamsResult, streamParamsDefault);

//Test Async Methods
// randomInts = [];
// for(let i = 0; i<1000;i++)
// {
//     randomInts.push(Math.floor(Math.random() * (100000-1))+1);
// }

// RtAudio.asyncTest(randomInts, function(err, result) {
//     let i = Math.floor(Math.random() * (result.length))
//     console.log("From JS Callback");
//     console.log("results.length: ", result.length);
//     console.log(`Random Result Value at ${i}:`, result[i]);
// });
// console.log("This should output before the result log");