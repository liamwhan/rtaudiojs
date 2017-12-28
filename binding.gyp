{
    "include_dirs": [
        "lib/rtaudio",
        "lib/rtaudio/include"
    ],
    'conditions': [
        ['OS=="win"', {

            "targets": [{
                'target_name': 'RTAJSDS',  # RtAudioJs using MS DirectSound API
                'defines': ['__WINDOWS_DS__'],
                'link_settings': {
                    'libraries': ['-lole32', '-lwinmm', '-ldsound', '-luser32']
                },
                "sources": [
                    "RtAudioJS.cc",
                    "lib/rtaudio/RtAudio.cpp"
                ],
                'configurations': {
                    'Release': {
                        'VCCLCompilerTool': {
                            'ExceptionHandling': '1',
                            'AdditionalOptions': ['/EHsc']
                        }
                    }
                }

            }, {
            # the WASAPI build will likely be removed before release as its less functional than DirectSound and I dont want multiple builds per platform.
                'target_name': 'RTAJSWASAPI',  # RtAudioJs using WASAPI
                'defines': ['__WINDOWS_WASAPI__'],
                'link_settings': {
                    'libraries': ['-lole32', '-lwinmm', '-lksuser', '-luuid']
                },
                "sources": [
                    "RtAudioJS.cc",
                    "lib/rtaudio/RtAudio.cpp"
                ],
                'configurations': {
                    'Release': {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'ExceptionHandling': '1',
                                'AdditionalOptions': ['/EHsc']
                            }
                        }
                    }
                }
            }]
        }],
        ['OS=="linux"', {
            'target_name': 'RtAudioJS',
            'defines': ['__LINUX_ALSA__'],
            'sources': [
                'RtAudioJS.cc',
                'lib/rtaudio/RtAudio.cpp'
            ],
            'cflags!': ['-fnoexceptions'], 
            'cflags_cc!': ['-fnoexceptions'],
            'link_settings': {
                'libraries': ['-lasound', '-lpthread']
            },
        }]
    ]
}
