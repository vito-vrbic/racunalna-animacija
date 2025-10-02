@echo off
REM Go to the directory of the script
cd /d %~dp0
cd ..

REM Go to the build folder
cd bin\build

REM Build the project (default configuration, usually Release)
cmake --build . --config Release