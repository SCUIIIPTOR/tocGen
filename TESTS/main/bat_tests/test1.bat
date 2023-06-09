@echo off
setlocal enabledelayedexpansion

set appPath=../tocGen.exe

echo TEST 1 running...

"%appPath%" "./example1.html"

echo TEST 1 completed.
echo.
pause