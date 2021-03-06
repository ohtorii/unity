﻿# file_searcher.exe について

- [はじめに](#はじめに)
- [プログラムの特徴](#プログラムの特徴)
- [使い方](#使い方)
- [出力ファイルのフォーマット](#出力ファイルのフォーマット)
- [ビルド手順](#ビルド手順)
- [プログラムの情報](#プログラムの情報)
- [連絡先](#連絡先)


# はじめに

Unityマクロから利用するファイル検索プログラムです。

ファイルを検索するプログラムは世の中に多くありますが、Unityマクロで使うのに適したプログラムが無かったため自作しました。


# プログラムの特徴
- 検索結果をUnityマクロに適したフォーマットで出力します。
- ファイル検索の動作に待ち時間を設定可能です、時間をかけてファイルを検索できます。（主にデバッグで使用します）
- ファイルとディレクトリを再帰的に検索できます。


# 使い方

## ファイルの検索

	>file_searcher.exe -mode file --path . --pattern * -o d:\temp\file.txt 

### 出力ファイルの内容（例）

	App.config	@action_filename	c:\project\file_searcher\App.config
	Application.cs	@action_filename	c:\project\file_searcher\Application.cs
	defines.cs	@action_filename	c:\project\file_searcher\defines.cs
	FileTraverser.cs	@action_filename	c:\project\file_searcher\FileTraverser.cs
	file_searcher.csproj	@action_filename	c:\project\file_searcher\file_searcher.csproj
	Ignore.cs	@action_filename	c:\project\file_searcher\Ignore.cs
	Properties\AssemblyInfo.cs	@action_filename	c:\project\file_searcher\Properties\AssemblyInfo.cs


## ディレクトリの検索

	>file_searcher.exe  -mode directory --path . --pattern * -o d:\temp\dir.txt 

### 出力ファイルの内容（例）

	c:\project\file_searcher\bin\	@action_directory	c:\project\file_searcher\bin\
	c:\project\file_searcher\obj\	@action_directory	c:\project\file_searcher\obj\
	c:\project\file_searcher\Properties\	@action_directory	c:\project\file_searcher\Properties\
	c:\project\file_searcher\bin\Debug\	@action_directory	c:\project\file_searcher\bin\Debug\
	c:\project\file_searcher\bin\Release\	@action_directory	c:\project\file_searcher\bin\Release\
	c:\project\file_searcher\obj\Debug\	@action_directory	c:\project\file_searcher\obj\Debug\
	c:\project\file_searcher\obj\Release\	@action_directory	c:\project\file_searcher\obj\Release\
	c:\project\file_searcher\obj\Debug\TempPE\	@action_directory	c:\project\file_searcher\obj\Debug\TempPE\
	c:\project\file_searcher\obj\Release\TempPE\	@action_directory	c:\project\file_searcher\obj\Release\TempPE\


## 待ち時間を指定する

1ファイル見付ける毎に500ミリ秒のウェイトを入れる。

	>file_searcher.exe --delay 500 -mode file --path . --pattern * -o d:\temp\file.txt 

ウェイトは主に非同期処理のデバッグで利用します。


## コマンドライン詳細

	>file_searcher.exe --help

	  -o, --output               Required. Output filename

	  -m, --mode                 Required. Path mode

	  -w, --working_directory    (Default: .) Working directory

	  -p, --path                 (Default: .) Search path

	  -n, --pattern              (Default: *) Search pattern

	  -h, --show_hidden_files    (Default: false) Show hidden files

	  -d, --delay                (Default: 0) Deley time (msec)

	  --help                     Display this help screen.

	  --version                  Display version information.


# 出力ファイルのフォーマット

ファイルフォーマットの詳細は以下ドキュメントを参照してください。

[internal.md](internal.md)


# ビルド手順

file_searcher.sln をVisualStudioで開いてビルドしてください。


# プログラムの情報

## ビルド環境

Visual Studio 2017

## 開発言語

C#

## フォルダ構成

	file_searcher
	├─file_searcher.sln                   //VisualStudioのソリューションファイル
	├─file_searcher                       //ソースコード
	└─packages
	    └─CommandLineParser.2.6.0         //外部ライブラリ


## 外部ライブラリ

コマンドライン引数を解析するために[CommandLineParser](https://www.nuget.org/packages/CommandLineParser/)
を利用しています。


# 連絡先

- <https://ohtorii.hatenadiary.jp>
- <https://twitter.com/ohtorii>
- <https://github.com/ohtorii>
