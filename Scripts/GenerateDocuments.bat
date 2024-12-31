@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Display parent directory for debugging
echo Parent directory: %parent_dir%

REM Display doxyfile path for debugging
echo Doxyfile path: %doxyfile_path%

REM List files in the parent directory for debugging
dir "%parent_dir%"

REM Check if doxyfile exists in the parent directory
if exist "%parent_dir%\Doxyfile" (

    set "doxyfile_path=%parent_dir%\Doxyfile"
    
    REM Back to parent folder
    cd ../
        
) else (

    REM Check if doxyfile exists in the current directory
    if exist "Doxyfile" (
    
        set "doxyfile_path=%cd%\Doxyfile"
        
    ) else (
    
        echo Doxyfile not found.
        PAUSE
        exit /b 1
        
    )
)

REM Call Python script to build solution
doxygen %doxyfile_path%

PAUSE