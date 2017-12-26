#Compiler command
CC=g++
MGW=i686-w64-mingw32-gcc
#Compiler flags
CFLAGS=-v -Wall -std=c++11
#Preprocessor definitions
DFLAG_MAC=-D__MACOSX_CORE__
DFLAG_NIX=-D__LINUX_ALSA__
DFLAG_WIN=-D__WINDOWS_DS__

#Binaries
BINARIES=./build/Debug/test.out
WIN_BINARY=./build/Debug/test.exe

#I/O Files
OUT=-o $(BINARIES)
SOURCES=./main.cpp lib/rtaudio/RtAudio.cpp
WIN_SOURCES=lib/rtaudio/include/asio.cpp lib/rtaudio/include/asiolist.cpp lib/rtaudio/include/asiodrivers.cpp lib/rtaudio/include/iasiothiscallresolver.cpp
INCLUDES=-Ilib/rtaudio -Ilib/rtaudio/include 

# Libaries and Include Paths
LIBFLAGS_MAC=-framework CoreAudio -framework CoreFoundation -lpthread
LIBFLAGS_NIX=-lasound -lpthread
LIBFLAGS_WIN=-lole32
mac: clean
	$(CC) $(CFLAGS) $(DFLAG_MAC) $(OUT) $(SOURCES) $(INCLUDES) $(LIBFLAGS_MAC)

linux: clean
	$(CC) $(CFLAGS) $(DFLAG_NIX) $(OUT) $(SOURCES) $(INCLUDES) $(LIBFLAGS_NIX)

win: clean
	$(MGW) $(CFLAGS) $(DFLAG_WIN) -o $(WIN_BINARY) $(SOURCES) $(WIN_SOURCES) $(INCLUDES) 

clean: 
	rm -f ./bin/*.out

