const RtAudio = require('./build/Release/RtAudioJs');


console.log(RtAudio);
let streamParams = new RtAudio.RtStreamParams(3, 2, 0, 48000);
console.log(streamParams)
streamParams.sampleRate = 44100;
console.log(streamParams);

 // Should construct successfully is all params passed as object or separately
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