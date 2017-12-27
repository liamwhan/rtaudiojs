{
    "targets": [
        {
            "target_name": "RtAudioDSound",
            "sources": [
                "RtAudioJS.cpp",
                "lib/rtaudio/RtAudio.cpp"
                ],
            "include_dirs": [
                "lib/rtaudio",
                "lib/rtaudio/include"
            ],
            'conditions': [
                ['OS=="win"', 
                {
                    'defines': [ '__WINDOWS_DS__'],
                    'link_settings': {
                        'libraries': ['-lole32', '-lwinmm' , '-ldsound']
                    }
                }]
            ]
        },
        {
            "target_name": "RtAudioWASAPI",
            "sources": [
                "RtAudioJS.cpp",
                "lib/rtaudio/RtAudio.cpp"
                ],
            "include_dirs": [
                "lib/rtaudio",
                "lib/rtaudio/include"
            ],
            'conditions': [
                ['OS=="win"', 
                {
                    'defines': [ '__WINDOWS_WASAPI__'],
                    'link_settings': {
                        'libraries': ['-lole32', '-lwinmm' , '-lksuser', '-luuid']
                    }
                }]
            ]
        }
        ]
}