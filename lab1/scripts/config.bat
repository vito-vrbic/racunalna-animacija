@echo off
cd /d %~dp0
cd ..

if not exist build mkdir build
cd build

:: Configure the project
cmake ..