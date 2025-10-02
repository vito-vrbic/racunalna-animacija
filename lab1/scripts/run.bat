@echo off
REM Go to the directory of the script
cd /d %~dp0
cd ..

REM Check if two arguments are provided
if "%~1"=="" (
    echo Usage: run.bat ^<object_file.obj^> ^<curve_file.crv^>
    exit /b 1
)
if "%~2"=="" (
    echo Usage: run.bat ^<object_file.obj^> ^<curve_file.crv^>
    exit /b 1
)

REM Run the executable with the two arguments
cd bin\output\Release
LAB1.exe "%~1" "%~2"