
const rtaudio = require('../build/Release/RtAudioWASAPI');

require('fs').writeFileSync('./test/fixtures/wasapi.json', JSON.stringify(rtaudio.deviceProbe()));
