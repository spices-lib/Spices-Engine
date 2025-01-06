@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Fetch project path and working path.
for %%f in ("%parent_dir%\bin\Release-windows-x86_64\SpicesTest\SpicesTest.exe") do set "project_path=%%f"
for %%f in ("%parent_dir%\SpicesTest") do set "working_path=%%f"

REM Return if not find valid project path and working path
if not defined project_path (
    echo project_path is not found
    PAUSE
    exit /b 1
)
if not defined working_path (
    echo working_path is not found
    PAUSE
    exit /b 1
)

REM Switch to working path.
cd /d "%working_path%"

REM RunProject
"%project_path%"

PAUSE