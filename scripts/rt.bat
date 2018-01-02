@echo off
set RTAJS=D:\rtaudiojs
doskey dv=pushd D:\rtaudiojs\build$tdevenv binding.sln$tpopd
doskey vsd=pushd %RTAJS%$tnode vsdebug.js$tpopd