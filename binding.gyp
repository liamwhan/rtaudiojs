{
    'conditions': [
        ['OS=="win"', {

            "targets": [{
                'target_name': 'RtAudioJs',  # RtAudioJs using MS DirectSound API
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "lib/rtaudio",
                    "lib/rtaudio/include"
                ],
                'defines': ['__WINDOWS_DS__'],
                'link_settings': {
                    'libraries': ['-lole32', '-lwinmm', '-ldsound', '-luser32']
                },
                'sources': [
                    'RtAudioJS.cc',
                    'RtStreamParams.cpp',
                    'lib/rtaudio/RtAudio.cpp',
                ],
                'configurations': {
                    'Release': {
                        'VCCLCompilerTool': {
                            'ExceptionHandling': '1',
                            'AdditionalOptions': ['/EHsc']
                        }
                    }
                }

            }]
        }]
        ,['OS=="linux"', {
            'targets': [{
                'target_name': 'RtAudioJs',
                'defines': ['__LINUX_ALSA__'],
                'sources': [
                    'RtAudioJS.cc',
                    'RtStreamParams.cpp',
                    'lib/rtaudio/RtAudio.cpp'
                ],
                'cflags!': ['-fno-exceptions'], 
                'cflags_cc!': ['-fno-exceptions'],
                'link_settings': {
                    'libraries': ['-lasound', '-lpthread']
                }
            }]
        }]
        ,['OS=="mac"', {
            'targets': [{
                'target_name': 'RtAudioJs',
                'defines': ['__MACOSX_CORE__'],
                'include_dirs': 
                [
                   "<!(node -e \"require('nan')\")",
                   "lib/rtaudio", 
                   "lib/rtaudio/include", 
                ],
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                },
                'sources': [
                    'RtAudioJS.cc',
                    'RtStreamParams.cpp',
                    'lib/rtaudio/RtAudio.cpp'
                ],
                'cflags!': ['-fno-exceptions'], 
                'cflags_cc!': ['-fno-exceptions'],
                'link_settings': {
                    'libraries': [
                        '-lpthread',
                        'CoreAudio.framework',
                        'CoreFoundation.framework'
                        ]
                }
            }]
        }]
    ]
}
