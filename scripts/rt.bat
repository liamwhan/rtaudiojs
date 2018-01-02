@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
call "D:\shell\shell.bat"
set RTAJS=D:\rtaudiojs
doskey dv=pushd D:\rtaudiojs\build$tdevenv binding.sln$tpopd
doskey vsd=pushd %RTAJS%$tnode vsdebug.js$tpopd