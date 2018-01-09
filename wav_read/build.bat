@echo off
REM Build Reader Only 
REM cl -FAsc -EHsc -Zi main.cpp

REM Build RtAudio  player
cls
cl -FAsc -EHsc -Zi -D__WINDOWS_DS__ main.cpp -I../lib/rtaudio/include ../lib/rtaudio/RtAudio.cpp User32.lib Ole32.lib WinMM.lib dsound.lib
@echo on