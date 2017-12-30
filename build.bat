@echo off
REM Build RtSawTooth.cpp Test executable using CL.exe (MSVS Build Tools)

REM vcvars.bat is not available in this repo, but it only calls `vcvarsall.bat x64` 
REM which bootstraps the current shell with Environment vars that MSVS uses 
REM (include paths, %PATH% modifications stuff like that) 
REM 
REM If you're using this file you can replace this line to call vcvarsall.bat on 
REM your system with something like:
REM call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
call D:\shell\vcvars.bat
pushd D:\rtaudiojs\build
cl -FAsc -Zi -EHsc -showIncludes -D__WINDOWS_DS__ -I../lib/rtaudio/include ../test/RtSawTooth.cpp ../lib/rtaudio/RtAudio.cpp User32.lib Ole32.lib WinMM.lib dsound.lib
popd