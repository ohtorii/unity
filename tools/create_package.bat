@echo off
REM
REM パッケージを作成するバッチファイル
REM
REM ＊使い方
REM create_package.bat バージョン番号 出力先ディレクトリ名
REM
REM ＊使用例
REM create_package.bat 1.2.3 c:\project\package
REM

setlocal
set TEMP_TIME=%time: =0%
set NOW=%date:/=%%TEMP_TIME:~0,2%%TEMP_TIME:~3,2%%TEMP_TIME:~6,2%
set ROOT_DIR=%TEMP%\tmp_hm_unity\%NOW%_%random%
set UNITY_ROOT_DIR=%ROOT_DIR%\unity

set RESULT=0
echo ROOT_DIR=%ROOT_DIR%

set ARG_VERSION=%1
set ARG_OUTDIR=%2

call :Main
exit /b %RESULT%



:Main
	REM 引数の確認
	call :CheckArguments
	if "%errorlevel%" NEQ "0" (
		set RESULT=1
		exit /b 1
	)

	REM 処理に必要なコマンドが存在するか調べる
	call :CheckCommand
	if "%errorlevel%" NEQ "0" (
		set RESULT=1
		exit /b 1
	)

	REM 作業用の一時ディレクトリを作る
	md "%ROOT_DIR%"
	if not exist "%ROOT_DIR%" (
		echo 一時ディレクトリを作成できませんでした
		set RESULT=1
		exit /b 1
	)
	REM 作業ディレクトリへ移動する
	pushd "%ROOT_DIR%"
	if "%errorlevel%" NEQ "0" (
		echo pushdコマンドが失敗しました
		set RESULT=1
		exit /b 1
	)

	REM メインの処理
	call :Core

	REM 元のディレクトリに戻る
	popd
	if "%errorlevel%" NEQ "0" (
		echo popdコマンドが失敗しました
		set RESULT=1
		exit /b 1
	)

	REM 後始末
	rmdir /S /Q "%ROOT_DIR%"

	exit /b %errorlevel%


REM 引数の確認
:CheckArguments
	if "%ARG_VERSION%" == "" (
		echo バージョン番号を指定してください。
		call :Usage
		set RESULT=1
		exit /b 1
	)
	if "%ARG_OUTDIR%" == "" (
		echo 出力ディレクトリを指定してください。
		call :Usage
		set RESULT=1
		exit /b 1
	)
	exit /b 0


REM バッチファイル中で使用するコマンドが存在するか調べる
:CheckCommand
	where git
	if "%errorlevel%" NEQ "0" (
		echo gitコマンドが見つかりません
		set RESULT=1
		exit /b 1
	)

	where 7z
	if "%errorlevel%" NEQ "0" (
		echo 7zコマンドが見つかりません
		set RESULT=1
		exit /b 1
	)
	exit /b 0



:Core
	REM UnityマクロをGitHubから取得する
	git clone --recursive --depth 1 "https://github.com/ohtorii/unity.git"
	if not exist "%UNITY_ROOT_DIR%" (
		echo gitコマンドが失敗しました。
		set RESULT=1
		exit /b 1
	)

	call :DeleteUnnecessaryFiles
	if "%errorlevel%" NEQ "0" (
		echo aaaaa
		set RESULT=1
		exit /b 1
	)

	REM zipで固める
	md "%ARG_OUTDIR%"
	if not exist "%ARG_OUTDIR%" (
		echo 出力ディレクトリを作成できませんでした
		set RESULT=1
		exit /b 1
	)
	
	7z.exe a -mx9 -mmt%NUMBER_OF_PROCESSORS% "%ARG_OUTDIR%\unity-%ARG_VERSION%.zip" "%UNITY_ROOT_DIR%\"
	if not exist  "%ARG_OUTDIR%\unity-%ARG_VERSION%.zip" (
		echo 7zコマンドが失敗しました
		set RESULT=1
		exit /b 1
	)
	
	exit /b 0



REM 不要なファイルを削除する
:DeleteUnnecessaryFiles
	rmdir /S /Q  "%UNITY_ROOT_DIR%\.git"
	if exist     "%UNITY_ROOT_DIR%\.git" (
		echo .git ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)
	
	rmdir /S /Q  "%UNITY_ROOT_DIR%\.github"
	if exist     "%UNITY_ROOT_DIR%\.github" (
		echo .git ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)
    del      "%UNITY_ROOT_DIR%\.gitattributes"
	if exist "%UNITY_ROOT_DIR%\.gitattributes" (
		echo tools ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)
    del      "%UNITY_ROOT_DIR%\.gitignore"
	if exist "%UNITY_ROOT_DIR%\.gitignore" (
		echo tools ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)
    del      "%UNITY_ROOT_DIR%\.gitmodules"
	if exist "%UNITY_ROOT_DIR%\.gitmodules" (
		echo tools ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)
	rmdir /S /Q  "%UNITY_ROOT_DIR%\tools"
	if exist     "%UNITY_ROOT_DIR%\tools" (
		echo tools ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)

	rmdir /S /Q  "%UNITY_ROOT_DIR%\help\images\resources"
	if exist     "%UNITY_ROOT_DIR%\help\images\resources" (
		echo help\images\resources ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)

	rmdir /S /Q  "%UNITY_ROOT_DIR%\internal\dll_project"
	if exist     "%UNITY_ROOT_DIR%\internal\dll_project" (
		echo internal\dll_project ディレクトリの削除に失敗しました。
		set RESULT=1
		exit /b 1
	)

	exit /b 0


:Usage
	echo;
	echo create_package.bat : Coopyright (c) 2020 ohtorii
	echo;
	echo ＊使い方
	echo create_package.bat バージョン番号 出力先ディレクトリ名
	echo
	echo ＊使用例
	echo create_package.bat 1.2.3 c:\project\package
	exit /b 0
