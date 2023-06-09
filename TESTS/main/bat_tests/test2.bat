@echo off
setlocal enabledelayedexpansion

set appPath=../tocGen.exe

echo TEST 2 running...

"%appPath%" "./example2.html"

echo TEST 2 completed.
echo.
pause