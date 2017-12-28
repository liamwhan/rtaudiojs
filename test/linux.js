const rtaudio = require('bindings')('RtAudioJS');

console.log(rtaudio.deviceProbe());
