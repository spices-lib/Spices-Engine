@echo off

REM Fetch parent folder.
set "parent_dir=%cd%\.."

REM Fetch project path and working path.
for %%f in ("%parent_dir%\bin\Debug-windows-x86_64\SpicesGame\SpicesGame.exe") do set "project_path=%%f"
for %%f in ("%parent_dir%\SpicesGame") do set "working_path=%%f"

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

REM Set environment variable
set PATH=C:/Program Files/Side Effects Software/Houdini 20.5.278/bin;E:\OpenGLProjects\Spices-Engine\/SpicesEngine/vendor/NvAftermath/lib/x64;E:\OpenGLProjects\Spices-Engine\/SpicesEngine/vendor/NvPerf/lib;C:/VulkanSDK/1.3.296.0/Lib/../Bin

REM RunProject
"%project_path%"

PAUSE