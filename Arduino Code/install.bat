@echo off 

for /D %%i in (.\*) do (
    echo %%i
    robocopy %%i %HOMEDRIVE%%HOMEPATH%\Documents\Arduino\%%i /s /e
)