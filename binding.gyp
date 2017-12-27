{
    "targets": [
        {
            "target_name": "RtAudio",
            "sources": [
                "RtAudioJS.cc",
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
                }],
                ['OS=="linux"',
                {
                    'cflags!': [ '-fno-exceptions' ],
                    'cflags_cc!': [ '-fno-exceptions' ],
                    'defines': [ '__LINUX_ALSA__'],
                    'link_settings': {
                        'libraries': ['-lasound', '-lpthread']
                    }
                }]
            ]
        }
        # ,{
        #     "target_name": "RtAudioWASAPI",
        #     "sources": [
        #         "RtAudioJS.cpp",
        #         "lib/rtaudio/RtAudio.cpp"
        #         ],
        #     "include_dirs": [
        #         "lib/rtaudio",
        #         "lib/rtaudio/include"
        #     ],
        #     'conditions': [
        #         ['OS=="win"', 
        #         {
        #             'defines': [ '__WINDOWS_WASAPI__'],
        #             'link_settings': {
        #                 'libraries': ['-lole32', '-lwinmm' , '-lksuser', '-luuid']
        #             }
        #         }]
        #     ]
        # }
        ]
}