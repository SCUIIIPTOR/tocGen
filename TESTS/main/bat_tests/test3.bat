@echo off
setlocal enabledelayedexpansion

set appPath=../tocGen.exe

echo TEST 3 running...

"%appPath%" "./example3.html"

echo TEST 3 completed.
echo.
pause