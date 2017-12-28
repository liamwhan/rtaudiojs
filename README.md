# RtAudioJS

**IMPORTANT:** Please read the [DISCLAIMER](#disclaimer) below
## Description
Exposes a real-time audio API to NodeJS, granting low-level IO access to system audio devices.

This library contains NodeJS bindings for the C++ real-time audio library [RtAudio](https://github.com/thestk/rtaudio) by Gary P. Scavone (McGill University). You can find the the [Documentation for RtAudio here](https://music.mcgill.ca/~gary/rtaudio).

**NOTE:** Currently only tested in the following environment: 
- Win 10 x64
- NodeJS v8.9.3 
- RtAudio compiled in MS DirectSound mode: `__WINDOWS_DS__` (read more [here](http://www.music.mcgill.ca/~gary/rtaudio/compiling.html))
- RtAudio compiled in Windows Audio Session API (WASAPI) mode: `__WINDOWS_WASAPI__` (read more [here](http://www.music.mcgill.ca/~gary/rtaudio/compiling.html))

## Installing Dependencies (Windows)
### Option 1 - Manual Install:
- NodeJS v8.9.3 (may work on earlier versions but is untested at this stage)
- MSVC Build Tools (Install Visual Studio and select C++ Desktop Development packages.)
- Python 2.x (required by `node-gyp`. I used v2.7.14)
- `node-gyp`: `npm i -g node-gyp`

### Option 2 - `windows-build-tools`
`windows-build-tools` is an `npm` package for acquiring `node-gyp` dependencies easily. It contains the required MSVS build tools and Python 2.7
- Run the following command from an **ELEVATED** (Run as Administrator) Command Prompt:
    - `npm i -g --production --add-python-to-path windows-build-tools`
- This will install the required Microsoft build tools, Python 2.7 and also add python to your `%PATH%` environment variable so you should be able to run `node-gyp` as soon as it completes.

### Additional Info 
- Due to licensing issues with Steinberg's ASIO driver, the RtAudio library must be cloned separate to this repo. 
- I have created an unmodified fork of the original RtAudio repo to prevent any breaking changes committed to the RtAudio repo from mangling this project. This will probably change in the future when I get CI setup.

## Building and running on Windows
1. Install [dependencies](#dependencies)
2. Clone this repo: 
    - `$ git clone https://github.com/hammus/rtaudiojs.git && cd rtaudiojs` 
3. Clone my fork of [RtAudio](https://github.com/hammus/rtaudio) to `lib/rtaudio`
    - `$ md lib && git clone https://github.com/hammus/rtaudio.git ./lib/rtaudio`
4. Configure the build (from project root e.g. `/path/to/rtaudiojs
    - `$ node-gyp configure`
5. Build the project 
    - `$ node-gyp build`
6. Run the test scripts 
    - Windows Direct Sound: `$ node test/dsound.js`
    - Windows WASAPI: `$ node test/wasapi.js`

If all went well you should see output similar the following (but with the devices on your local machine):
```javascript
[ { probed: true,
    name: 'Default Device',
    outputChannels: 2,
    inputChannels: 2,
    duplexChannels: 2,
    isDefaultOutput: true,
    isDefaultInput: true,
    sampleRates:
     [ 4000,
       5512,
       8000,
       9600,
       11025,
       16000,
       22050,
       32000,
       44100,
       48000,
       88200,
       96000,
       176400,
       192000 ],
    preferredSampleRate: 48000 } ]

```

## Disclaimer
This is a very new project. It is currently being actively developed and should be considered unstable. It has only been tested on Windows 10 x64, using Node v8.9.3 with RtAudio compiled in DirectSound mode (`__WINDOWS_DS__`) 

It will not be published to the `npm` registry until v1.0 which should be considered the first stable version.

## RtAudio License
[RtAudio](https://music.mcgill.ca/~gary/rtaudio) is licensed under a variant of the MIT License, please see [here](http://www.music.mcgill.ca/~gary/rtaudio/license.html) for more information.

## RtAudioJS License (MIT)
Copyright (c) 2017-2018 Liam Whan

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
