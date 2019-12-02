# Unity

![Maintenance](https://img.shields.io/maintenance/yes/2020.svg)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Coverity](https://img.shields.io/coverity/scan/19629.svg)](https://scan.coverity.com/projects/ohtorii-unity)

- [Unity](#unity)
- [はじめに](#はじめに)
- [スクリーンショット（準備中）](#スクリーンショット準備中)
- [マクロのデザイン](#マクロのデザイン)
- [画面の名称](#画面の名称)
- [操作方法](#操作方法)
- [マクロの導入方法](#マクロの導入方法)
- [動作環境](#動作環境)
- [ダウンロード](#ダウンロード)
- [その他](#その他)
- [謝辞](#謝辞)
- [連絡先](#連絡先)
- [寄付](#寄付)

# はじめに

作成途中なのでまだ動かないです。


[unite.vim](https://github.com/Shougo/unite.vim)の秀丸エディタ版。

Emacsでいうところの[anything.el](http://emacs.rubikitch.com/anything/)的存在。

# スクリーンショット（準備中）


## 文字列の挿入

![文字列の挿入](help/images/insert.gif "文字列の挿入")

## ファイル履歴(file_mru)からファイルを見付けて開く

![file_mru](help/images/file_mru_open.gif "file_mru")

## ファイル履歴(file_mru)の削除

![file_mru](help/images/file_mru_remove.gif "file_mru")

## プロセスのkill

![process](help/images/process_kill.gif "process")

## auto preview オプション

***previewアクション**を自動的に呼び出します。
![autopreview](help/images/file_autopreview.gif "autopreview")


# マクロのデザイン

関心対象（オブジェクト）を選択してから操作（アクション）を行うデザインをテキストエディタ上で行います。
![オブジェクトベースUI](help/images/ooui.png "オブジェクトベースUI")


## 処理の流れ

![処理の流れ](help/images/oogui_summary.png "処理の流れ")


## source（ソース）とは

候補(candidates)を集めてくる秀丸マクロのこと。

標準のソース一覧

- directory
- directroy_mru
- directroy_recursive
- file
- file_mru
- file_recursive
- emoji
- process
- window
- など

もちろんユーザー作成のソースも自由に追加できます。


## kind（カインド）とは

候補（candidates）の型に対応した操作(action)をまとめた秀丸マクロです。

標準のカインド一覧

- common
- file
- file_base
- cdable
- process
- window
- など

もちろんユーザー作成のカインドも自由に追加できます。

カインドは複数のactionを持っています、例えばcommonカインドは以下のアクションを持っています。

	(例)
	commonカインド
		nopアクション
		yancアクション
		echoアクション
		insertアクション
		  :
		  :
		など


## action(アクション)とは

選択した候補に対して行う操作のことです。

アクションは幾つかのフラグを持っています。

- 複数の候補を選択可能か
- ユーザーが編集中のテキストを変更するかどうか
- 操作後すぐに終了するか
- 操作後に新たな候補を生成するか

マクロの内部処理と密に連携しているためユーザー定義のフラグは追加できません。


# 画面の名称

二つの画面（バッファ）があります。

- 検索バッファ
- 候補バッファ

![インターフェース](help/images/interface.png "インターフェース")

## 検索バッファ

検索の候補を入力します。

## 候補バッファ

絞り込み検索した結果をリアルタイムで表示し、操作対象の候補（赤文字の行）を選択します。

# 操作方法

## 検索バッファ

行編集が主なので概ね[Bash](https://www.google.com/search?q=bash)に準拠しています。

### カーソル移動

|キー|説明|備考|
|--|--|--|
|← or Ctrl-b|カーソル左||
|→ or Ctrl-f|カーソル右||
|Home|先頭へ||
|End|最後へ||


### テキスト編集

|キー|説明|備考|
|--|--|--|
|Del or Ctrl-d or Ctrl-Backspace|Delete||
|Ctrl-t|Transpose||
|Ctrl-k|KillLine||
|Ctrl-u|LineDiscard||
|Backspace or Ctrl-h|Backspace||


## 候補バッファ

|キー|説明|備考|
|--|--|--|
|↑|前候補へ移動||
|↓|次候補へ移動||
|PageUp|１ページ前へ移動||
|PageDown|１ページ後ろへ移動||
|Ctrl-space|候補をマークする|複数の候補を選択するとき利用します|
|Ctrl-a|候補を全て選択する||
|Enter|ディフォルトアクションを実行する||
|Esc|一つ前のバッファに戻る||


## 共通

|キー|説明|備考|
|--|--|--|
|Ctrl-w|タブを分離する|タブモード時のみ有効。auto-previewと組み合わせると便利|
|Ctrl-c or Ctrl-q|マクロを終了する||

# マクロの導入方法

まずは、動作確認を行った安定バージョンをダウンロードして下さい。

https://github.com/ohtorii/unity/releases

masterブランチは動作確認を行っていないため動作しないです。（仕事じゃないので気楽に開発してます☺）

## ディレクトリ構成

全ファイルとフォルダを秀丸エディタのスクリプトディレクトリにコピーしてください。

コピー後のディレクトリ構成

```
hidemaru-editor-script-directory
 └unity
    ├─help
    ├─internal
    ├─kinds
    └─sources
```

## ショートカットキー

`unity_dialog.mac` をショートカットキーに割り当てて下さい。

（例）
- Ctrl-@
- Ctrl-:
- Ctrl-;
- Ctrl-.
- Ctrl-/

コマンド実行っぽいキーを列挙してみました。

## unity_dialog.macを起動する

ショートカットキーを押下して`unity_dialog.mac` を起動すると、
![GUI](help/images/gui.png "GUI")

↑こんなダイアログが開きます。

ソース(file_mruとかprocessとか)を選んでEnterキーを押下すると絞り込み検索が始まります😙


## いろいろな方法でUnityマクロを呼び出す

以下の表からご自身の都合に合うマクロをショートカットキーに割り当ててご利用ください。

汎用型マクロと特化型マクロの二通りを用意しました。

|ファイル名|説明|ソースの複数選択|オプション選択|
|--|--|--|--|
|unity_dialog.mac						|ダイアログから複数ソースとオプションを指定可能。全機能を利用可能|○|○|
|unity_menu.mac							|メニューからソースを一つ選択して呼び出す。ソースを一つ選択することに特化|×|×|
|unity_menu_manual.mac					|利用頻度の高いソースをメニュー化する例|×|×|
|unity_source_directory.mac				|directroyソースを指定して起動|×|×|
|unity_source_directory_mru.mac			|directroy_mruソースを指定して起動|×|×|
|unity_source_directory_recursive.mac	|directroy_recursiveソースを指定して起動|×|×|
|unity_source_emoji.mac					|emojiソースを指定して起動|×|×|
|unity_source_file.mac					|fileソースを指定して起動|×|×|
|unity_source_file_mru.mac				|file_mruソースを指定して起動|×|×|
|unity_source_file_recursive.mac		|file_recursiveソースを指定して起動|×|×|
|unity_source_hilight.mac				|hilightソースを指定して起動|×|×|
|unity_source_process.mac				|processソースを指定して起動|×|×|
|unity_source_window.mac				|windowソースを指定して起動|×|×|

それぞれ数行のマクロで理解しやすいと思います、上級ヒデマラーの皆様におかれましてはカスタマイズしてお使いください。


# 動作環境

- 秀丸エディタ ver xxx 以降

秀丸マクロの***新命令***を利用しているため***必ず新しいバージョン***をお使いください。

## 動作確認を行った環境

ご参考までに。

- Windows 10 64bit 
- 秀丸エディタ 64bit ver xxx



# ダウンロード

こちらから動作確認済みのパッケージをダウンロードして下さい。

https://github.com/ohtorii/unity/releases

## 注意
masterブランチを取得しても多分動作しないです。

ブランチを作らずに気楽に開発してます。（仕事じゃないしね😉）

## バージョン番号のルール

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

## 参考資料

- http://emacs.rubikitch.com/sd1503-helm/
- https://gist.github.com/Shougo/7fc95389ab6fddac7f78deb6ce42e82a


# 謝辞

## 思想とソースコードをかなり参考にさせてもらいました😘😘😘
- [unite.vim](https://github.com/Shougo/unite.vim)
- [anything.el](http://emacs.rubikitch.com/anything/)

## 作者に感謝
- [田楽DLL 32bit](http://www.ceres.dti.ne.jp/~sugiura/hidemaru/macros/dgserver/#download64)
- [田楽DLL 64bit](http://htom.in.coocan.jp/macro/macro_dll.html#label-5)

# 連絡先

- <https://ohtorii.hatenadiary.jp>
- <https://twitter.com/ohtorii>
- <https://github.com/ohtorii>

# 寄付

もし気に入ってくれたらSteamからゲームを寄付してくれるとうれしいです😘
- https://store.steampowered.com/wishlist/id/ohtorii

開発の励みになります。
