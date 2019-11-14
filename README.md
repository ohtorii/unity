# Unity

![Maintenance](https://img.shields.io/maintenance/yes/2020.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

作成途中なのでまだ動かないです。


[unite.vim](https://github.com/Shougo/unite.vim)の秀丸エディタ版。

Emacsでいうところの[anything.el](http://emacs.rubikitch.com/anything/)的存在。

# スクリーンショット（準備中）


# マクロの思想

後で書く

## source（ソース）とは

## kind（カインド）とは



# マクロの導入方法（準備中）

まずは、動作確認を行った安定バージョンをダウンロードして下さい。
https://github.com/ohtorii/unity/releases

masterブランチを取得しても多分動作しないです。（動作確認を行っていないため）

## ディレクトリ構成

全ファイルとフォルダを秀丸エディタのスクリプトディレクトリにコピーしてください。

コピー後のディレクトリ構成

	unity
	  ├─bin
	  ├─dll_project
	  ├─help
	  ├─internal
	  ├─kinds
	  └─sources

## ショートカットキー割り当て

`unity_dialog.mac` をショートカットキーに割り当てて下さい。

### 割り当ての例

- Ctrl-@
- Ctrl-:

私はコマンド実行っぽいキーに割り当ててます。


## いろいろな方法でUnityを呼び出す

以下の表からご自身の都合に合うマクロをショートカットキーに割り当ててご利用ください。

|ファイル名|説明|ソース選択|オプション選択|
|--|--|--|--|
|unity_dialog.mac						|ダイアログから複数ソースとオプションを指定可能で最も汎用的|複数|○|
|unity_menu.mac							|メニューからソースを一つだけ選択して呼び出す|一つ|×|
|unity_menu_manual.mac					|頻繁に利用するソースのみメニュー化する例|一つ|×|
|unity_source_directory.mac				|directroyソースを指定して起動||×|
|unity_source_directory_mru.mac			|directroy_mruソースを指定して起動|一つ|×|
|unity_source_directory_recursive.mac	|directroy_recursiveソースを指定して起動|一つ|×|
|unity_source_emoji.mac					|emojiソースを指定して起動|一つ|Ｘ|
|unity_source_file.mac					|fileソースを指定して起動|一つ|Ｘ|
|unity_source_file_mru.mac				|file_mruソースを指定して起動|一つ|Ｘ|
|unity_source_file_recursive.mac		|file_recursiveソースを指定して起動|一つ|Ｘ|
|unity_source_hilight.mac				|hilightソースを指定して起動|一つ|Ｘ|
|unity_source_process.mac				|processソースを指定して起動|一つ|Ｘ|
|unity_source_window.mac				|windowソースを指定して起動|一つ|Ｘ|


# 動作環境

準備中


# ダウンロード

こちらから動作確認済みのパッケージをダウンロードして下さい。

https://github.com/ohtorii/unity/releases

### 注意
masterブランチを取得しても多分動作しないです。<br>
ブランチを作らずに気楽に開発してます。（仕事じゃないしね😉）

### バージョン番号のルール

バージョン番号の表記方法

バージョン番号は version 1.2.3 のように表記され、それぞれ major.minor.revision を表します。

|番号|説明|
|:--:|:--:|
|major|互換性が失われる大きな変更を表します|
|minor|機能追加のように互換性のある更新を表します|
|revision|バグ修正のように機能そのものに変化が無い軽微な更新を表します|

# その他

## マクロ名について

なぜ、マクロ名をUniteではなくUnityにしたのか？

unite.vimは***コマンド***なのでunite(動詞)の方が自然ですが、秀丸エディタでは***マクロ名***なので名詞にしました。

ゲームエンジンのUnityと同名ですが、それを言い出すとVim-UniteはUnityの大規模イベント（Unite）と同名なので…まあいっか😙とそっと蓋を閉じました😙

|英単語|品詞|説明|
|:--:|:--:|:--:|
|unite|動詞|vim|
|unity|名詞|秀丸エディタ|

## unite.vimとunityマクロの機能対応

unite.vimを秀丸化して移植しています、そのためオリジナル(VIM)の動作と異なる部分があります。

詳細は以下のリンク先を参照してください。（主にVimmer方向け説明です）

[vim_to_hidemaru.md](help/vim_to_hidemaru.md)


# 謝辞

- [unite.vim](https://github.com/Shougo/unite.vim)
- [anything.el](http://emacs.rubikitch.com/anything/)

思想とソースコードをかなり参考にさせてもらいました😘😘😘

# 連絡先

- <https://ohtorii.hatenadiary.jp>
- <https://twitter.com/ohtorii>
- <https://github.com/ohtorii>
