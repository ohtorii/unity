@echo off
REM 
REM Coverity検査に必要なファイルを生成する
REM 

setlocal

set COV_BUILD=C:\cov-analysis-win64-2019.03\bin\cov-build.exe

REM フォルダ名は変えないこと(Coverityのマニュアルに説明あり)
set COV_DIR=cov-int

REM (Memo)VS2019だとcob-buildでエラー発生、VS2017を利用中。
set SOLUTION=ohtorii_tools-2017.sln
set BUILD_COMMAND=msbuild %SOLUTION% /t:clean;rebuild /p:Configuration=Release;Platform=x64;PostBuildEventUseInBuild=false
if not exist %COV_BUILD% (
    echo cov-build.exe が見つかりません
    exit /b 1
)

cd %~dp0
call :Main
exit /b %errorlevel%

:Main
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
	REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
	if %errorlevel% neq 0 (
		echo VsDevCmd.batの実行に失敗しました
	)
	%COV_BUILD% --dir %COV_DIR% %BUILD_COMMAND%
	if %errorlevel% neq 0 (
		echo cov-buildが失敗しました
		exit /b 1
	)
	REM 検査結果をWEBへアップするためにtarで固める
	tar czvf ohtorii_tools.tgz %COV_DIR%
	exit /b 0
