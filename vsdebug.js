const RtAudio = require('bindings')('RtAudioJs');

RtAudio.deviceProbe();
RtAudio.setStreamParams({deviceId: 0, nChannels: 2, sampleRate: 48000});
console.log(RtAudio.getStreamParams());