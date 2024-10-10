@echo off

:: Iter all GPU device configurations and Generate .h file.
for /d %%i in (pub/*) do (
    call python hud_configurations_generator.py --chip %%i --outDir ../include --yamlPaths pub/%%i
)

IF %ERRORLEVEL% NEQ 0 (
    PAUSE
)
