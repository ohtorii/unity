# 内部実装について

- [はじめに](#はじめに)
- [sourceについて](#sourceについて)
- [kindについて](#kindについて)
- [固有名詞](#固有名詞)
- [アクションの上書きについて](#アクションの上書きについて)
- [カインドの継承順序について](#カインドの継承順序について)
- [アクションのファイルパスについて](#アクションのファイルパスについて)
- [カインドの種類](#カインドの種類)
- [候補のフォーマットについて](#候補のフォーマットについて)
- [コールバックの登録](#コールバックの登録)
- [連絡先](#連絡先)

# はじめに

このドキュメントはUnityマクロの内部実装について書いています。

作成途中です。


# sourceについて

## candidate_type

候補を返す方法を指定する

|値|説明|処理|
|--|--|--|
|function|DLL側の関数を利用して候補を作成する|同期|
|string|候補を改行区切りのリストで返す|同期|
|file|候補をファイルから作成する|同期|
|async_file|候補をファイルから作成する|非同期|

## description

説明を一行で記述する。

## default_kind

ディフォルトで呼び出すカインド


# kindについて

## propertyの属性
|属性|説明|ディフォルト値|
|description|説明|""|
|default_action|ディフォルトのアクション名|""|
|base_kind|基底カインド|""|


## アクションの属性

|属性|説明|ディフォルト値|
|--|--|--|
|is_quit|アクション実行後に終了するかどうか|false|
|is_multi_selectable|複数選択に対応したアクションかどうか|false|
|is_edit|テキストを編集するアクションかどうか|false|
|is_start|アクション実行後に候補を生成しアクションを開始するかどうか|false|
|is_reget_candidates|アクション実行後に候補を再取得するかどうか|false|

# 固有名詞

## 秀丸エディタ

|固有名詞|説明|備考|
|--|--|--|
|行番号|1開始の値||
|行インデックス|0開始の値|行インデックス=行番号-1|
 


# アクションの上書きについて

アクションは別のアクションを継承することが出来ます。

## 例 1

通常の使用例。

	kinds\file.mac
		default_action=open

	sources\file.mac
		default_kind=file		<- fileカインドのopenを呼ぶ

## 例 2

上書きした場合の例。

	kinds\file.mac
		default_action=open

	sources\file.mac
		default_kind=file
		default_action=echo		<- commonアクションのechoを呼ぶ

## 優先順位

|種類|優先度|
|--|--|
|kind|低|
|source|高|

# カインドの継承順序について

	base_kind=jump_list file
	default_action=open

jump_listとfileはそれぞれopenアクションを持っています、上記例ではjump_listのopenが呼ばれます。

## 仕様

base_kind=最高優先度 次の優先度 ... 最低優先度


# アクションのファイルパスについて

アクションのパス（ディレクトリ、ファイル）を設定取得することが出来ます。

## 設定(source側)
 ##success=dllfuncw(#g_dll_ohtorii_tools, "SetCandidateActionPath", $$file_name);

## 取得(kind側)
$$text=dllfuncstrw(#g_dll_ohtorii_tools,"GetSelectionActionPath",##first_item_index);

#  カインドの種類

candidateの種類

- string
- file
- async_file
- function

## string

候補を文字列から生成します。

### サンプル

sources\emoji.macを参照してください。


## file

候補をファイル（同期）から生成します。

## async_file

候補をファイル（非同期）から生成します。
時間のかかる処理に向いています。(c:\windows以下のファイルを列挙する場合など)

### サンプル
sources\file_recursive.mac を参照してください。


## function

候補をohtorii_toolsの関数を利用して生成します。
関数を直接利用するので最も細かく制御できます。

### サンプル

process.mac を参照してください。


# 候補のフォーマットについて

candidate_typeが以下の時のフォーマットについて。
- string
- file
- async_file

## 文字コード

UTF16LE-BOM

## ファイルフォーマット

ファイルフォーマットは2種類あります。

### ファイル内容の例１（シンプル）

	候補テキスト１
	候補テキスト２
	候補テキスト３
		...
	候補テキストN


### ファイル内容の例２（汎用）
	
候補に続いてタブ区切りで追加情報を並べます。

	候補テキスト１	action名１	アクションに紐付いたテキスト１
	候補テキスト２	action名２	アクションに紐付いたテキスト２
	候補テキスト３	action名３	アクションに紐付いたテキスト３
		...
	候補テキストN	action名４	アクションに紐付いたテキストN

#### action名について

以下のいずれかを指定します。
- @action_directory
- @action_filename
- @description

## サンプル

|candidate_type|サンプルのファイル名|
|--|--|
|string|sources\emoji.mac|
|file|後で用意する|
|async_file|sources\file_recursive.mac|


# コールバックの登録

ユーザー定義のコールバックを登録できます。

## コールバックの種類

- Unityマクロ終了直前

## 使用例

\kinds\window.mac
jump: ラベルの処理を参照してください。


## 制限

コールバックの種類毎に制限があります。

### Unityマクロ終了直前

以下のDLLは解放済みなので呼ぶことは出来ません。

- ohtorii_tools


# 連絡先

- <https://ohtorii.hatenadiary.jp>
- <https://twitter.com/ohtorii>
- <https://github.com/ohtorii>
