if [ "$(uname)" == "Darwin" ] 
then
    g++ -Wall -D__MACOSX_CORE__ -o ./build/saw ./test/RtSawTooth.cpp ./lib/rtaudio/RtAudio.cpp -I./lib/rtaudio/RtAudio.h -framework CoreAudio -framework CoreFoundation -lpthread
elif [ "$(uname)" == "Linux" ]
then
    g++ -Wall -D__LINUX_ALSA__ -o ./build/saw ./test/RtSawTooth.cpp ./lib/rtaudio/RtAudio.cpp -I./lib/rtaudio/RtAudio.h -lasound -lpthread
fi