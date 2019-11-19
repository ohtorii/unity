@echo off
REM 
REM DLLをマクロディレクトリへコピーする
REM 
copy /Y /B "%~dp0x64\Release\ohtorii_tools_x64.dll" "%~dp0..\..\dll\"
copy /Y /B "%~dp0Release\ohtorii_tools_x86.dll"     "%~dp0..\..\dll\"
