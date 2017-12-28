# RtAudioJS
**IMPORTANT:** CURRENTLY UNSTABLE. Please read the [DISCLAIMER](#disclaimer) below

# Contents
- [Description](#description)
- [JS API Reference](#javascript-api-reference)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)
- [Licenses](#rtaudio-license)

# Description
Exposes a real-time audio API to NodeJS, granting low-level IO access to system audio devices.

This library contains NodeJS bindings for the C++ real-time audio library [RtAudio](https://github.com/thestk/rtaudio) by Gary P. Scavone (McGill University). You can find the the [Documentation for RtAudio here](https://music.mcgill.ca/~gary/rtaudio).

**NOTE:** Currently only tested in the following environment: 
- Win 10 x64
- NodeJS v8.9.3 
- RtAudio compiled in MS DirectSound mode: `__WINDOWS_DS__` (read more [here](http://www.music.mcgill.ca/~gary/rtaudio/compiling.html))
- RtAudio compiled in Windows Audio Session API (WASAPI) mode: `__WINDOWS_WASAPI__` (read more [here](http://www.music.mcgill.ca/~gary/rtaudio/compiling.html))

# Javascript API Reference

RtAudio
=======

## Types

-----
### RtDeviceInfo
An object with details for an audio device on the host system. 
| **Member**    | **Type** | **Description**    |
|---------------|----------|--------------------|
| `probed`      | `Boolean`| `true` if the probe was successful, `false` otherwise.     |
| `name`        | `String` | The device name    |
| `outputChannels`  | `Number` | An integer representing the number of output channels  |
| `inputChannels`   | `Number` | An integer representing the number of input channels   |
| `duplexChannels`  | `Number` | An integer representing the number of duplex channels  |
| `isDefaultOutput` | `Boolean`| `true` if the host system reports this device as the default output, `false` otherwise. |
| `isDefaultInput`  | `Boolean`| `true` if the host system reports this device as the default input, `false` otherwise. |
| `sampleRates`     | `Array<Number>` | An array of sample rates supported by the device |
| `preferredSampleRate` | `Number` | The preferred sample rate of the device (if any) reported by the host. |

[Top](#contents)
## Methods

-----
### RtAudio.deviceProbe()
| | |
|-----------------|---------------------------------------------------------------------------|
| **Description** | Probes the OS for available audio devices (sound cards, microphones etc.) and returns an object with details for each probed device|
| **Parameters**  | None |
| **Return Type** | `Array<`[`RtDeviceInfo`](#rtdeviceinfo)`>` An array of [`RtDeviceInfo`](#rtdeviceinfo) for each successfully probed audio device |

#### Examples
```javascript

    const RtAudio = require('RtAudioJs');
    console.log(RtAudio.deviceProbe());

```

Example Output:
```javascript
[
    {
        "probed": true,
        "name": "Default Device",
        "outputChannels": 2,
        "inputChannels": 2,
        "duplexChannels": 2,
        "isDefaultOutput": true,
        "isDefaultInput": true,
        "sampleRates": [
            4000,
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
            192000
        ],
        "preferredSampleRate": 48000
    },
    {
        "probed": true,
        "name": "Line (2- Steinberg UR44)",
        "outputChannels": 2,
        "inputChannels": 2,
        "duplexChannels": 2,
        "isDefaultOutput": false,
        "isDefaultInput": false,
        "sampleRates": [
            4000,
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
            192000
        ],
        "preferredSampleRate": 48000
    }
]
```
[Top](#contents)

# Contributing
Contributions are welcome, instructions for setting up a development environment on Windows are below.
## Installing Windows Dependencies 
### Option 1 - Manual Install:
- NodeJS v8.9.3 (may work on earlier versions but is untested)
- MSVC Build Tools (Install Visual Studio with C++ Development enabled.)
- Python 2.x (required by `node-gyp`. I used v2.7.14)
- `node-gyp`: `npm i -g node-gyp`

## Option 2 - `windows-build-tools`
`windows-build-tools` is an `npm` package that contains MSVC build tools and Python 2.7
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
