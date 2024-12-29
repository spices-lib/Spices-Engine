@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Display parent directory for debugging
echo Parent directory: %parent_dir%

REM Fetch solution path.
for %%f in ("%parent_dir%\Doxyfile") do set "doxyfile_path=%%f"

REM Display doxyfile path for debugging
echo Doxyfile path: %doxyfile_path%

REM List files in the parent directory for debugging
dir "%parent_dir%"

REM Return if not find a solution file
if not defined doxyfile_path (
    echo doxyfile is not found
    PAUSE
    exit /b 1
)

REM Back to parent folder
cd ../

REM Call Python script to build solution
doxygen %doxyfile_path%

PAUSE