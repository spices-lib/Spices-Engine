@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Display parent directory for debugging
echo Parent directory: %parent_dir%

REM Fetch solution path.
for %%f in ("%parent_dir%\*.sln") do set "solution_path=%%f"

REM Display solution path for debugging
echo Solution path: %solution_path%

REM List files in the parent directory for debugging
dir "%parent_dir%"

REM Return if not find a solution file
if not defined solution_path (
    echo solution is not found
    PAUSE
    exit /b 1
)

REM Call Python script to build solution
call python Build.py --target "%solution_path%" --configuration Release --platform x64

if %ERRORLEVEL% NEQ 0 (
    PAUSE
)