@echo off
setlocal

set COV_BIN=C:\cov-analysis-win64-2019.03\bin
set BUILD_COMMAND=msbuild ohtorii_tools.sln /t:clean;rebuild /p:Configuration=Release;Platform=x64;PostBuildEventUseInBuild=false
cd %~dp0
call :Main
exit /b %errorlevel%

:Main
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
	%COV_BIN%\cov-build --dir cov-int %BUILD_COMMAND%
	tar czvf ohtorii_tools.tgz cov-int
	exit /b 0
