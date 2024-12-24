@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Fetch solution path.
for %%f in ("%parent_dir%\*.sln") do set "solution_path=%%f"

REM Return if not find a solution file
if not defined solution_path (
    echo solution is not found
    PAUSE
    exit /b 1
)

REM Call Python script to build solution
call python Build.py --target "%solution_path%" --configuration Debug --platform x64

PAUSE