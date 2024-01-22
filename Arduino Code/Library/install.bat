@echo off 

for /D %%i in (.\*) do (
    echo %%i
    robocopy %%i %HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\%%i /s /e
)