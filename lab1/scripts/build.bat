@echo off
cd /d %~dp0
cd ..\build

:: Incremental build
cmake --build . --config Release -- /m

:: Run executable
cd Release
lab1.exe
