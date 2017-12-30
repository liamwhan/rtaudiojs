const RtAudio = require('bindings')('RtAudioJs');

RtAudio.deviceProbe();
RtAudio.setStreamParams({deviceId: 0, nChannels: 2, sampleRate: 48000});
console.log(RtAudio.getStreamParams());

//Test Async Methods
randomInts = [];
for(let i = 0; i<1000;i++)
{
    randomInts.push(Math.floor(Math.random() * (100000-1))+1);
}

RtAudio.asyncTest(randomInts, function(err, result) {
    let i = Math.floor(Math.random() * (result.length))
    console.log("From JS Callback");
    console.log("results.length: ", result.length);
    console.log("Random Result Value:", result[i]);
});