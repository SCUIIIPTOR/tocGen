@echo off
setlocal enabledelayedexpansion

set appPath=../tocGen.exe

echo TEST running...

"%appPath%" "./example.html"

echo TEST completed.
echo.
pause