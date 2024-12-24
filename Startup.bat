@echo off

REM Display current directory for debugging
echo Current directory: %cd%

REM List files in the current directory for debugging
dir

REM Check if premake5.exe exists
if not exist vendor\premake\premake5.exe (
    echo premake5.exe not found
    PAUSE
    exit /b 1
)

REM Call premake5.exe to generate the solution
call vendor\premake\premake5.exe vs2022
if %ERRORLEVEL% NEQ 0 (
    echo premake5.exe failed
    PAUSE
    exit /b 1
)

REM Display current directory after generation for debugging
echo Current directory after generation: %cd%

REM List files in the current directory after generation for debugging
dir