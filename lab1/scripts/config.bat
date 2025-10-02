@echo off
REM Go to the directory of the script
cd /d %~dp0
cd ..

REM Create bin/build folder if it doesn't exist
if not exist bin\build mkdir bin\build
cd bin\build

REM Configure the project with CMake pointing to the project root
cmake ../..