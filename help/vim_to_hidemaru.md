# unite.vimとの対応表

unityとunite.vimのソース＆アクション対応表です。

# ソース一覧

|機能|説明|実装|
|:--:|:--:|:--:|
|file				|カレントディレクトリのファイルとディレクトリ一覧||
|file/new			|ファイルの新規作成||
|file_mru			|最近開いたファイルの一覧|○|
|file_rec			|カレントディレクトリの全てのファイルの一覧||
|file_rec/async		|file_rec の非同期処理版||
|directory			|カレントディレクトリのディレクトリ一覧||
|directory/new		|ディレクトリの新規作成||
|directory_mru		|最近開いたディレクトリの一覧||
|tab				|タブの一覧||
|source				|unite-source の一覧||
|window				|現在のウィンドウのタブ一覧|○|
|command			|定義されているコマンドの一覧||
|function			|Vim の組み込み関数と unite.vim、neocomplcache で定義されている関数の一覧||
|mapping			|定義されているキーマッピングの一覧||
|grep				|外部コマンド "grep" の結果を出力||
|find				|外部コマンド "find" の結果を出力||
|menu				|g:unite_source_menu_menus に登録されてあるメニューの一覧||
|history/yank		|ヤンクされたワードの一覧||
|process			|起動しているプロセスの一覧||
|bookmark			|ブックマークの一覧（秀丸マクロに命令が存在しない）||
|runtimepath		|runtimepath に追加されているパスの一覧||
|launcher			|$PATH にある実行ファイルの一覧||
|alias				|g:unite_source_alias_aliases の一覧||
|output				|Vim のコマンドの出力結果を出力する（e.g. :mes）||
|line/fast			|10000行以上のバッファ時に使用する line||
|jump				|:jump の結果の出力||
|change				|:change の結果を出力||
|jump_point			|file:line に一致するデータを出力||
|file_point			|ファイル名か URI に一致するデータを出力する||


## 今後秀丸エディタ化するソース名
|機能|説明|実装|
|:--:|:--:|:--:|
|buffer				|開かれているバッファの一覧||
|buffer_tab			|現在のタブで開かれたバッファの一覧||
|line				|現在のバッファの行を出力||
|vimgrep			|:vimgrep の結果を出力||
|register			|レジスタの一覧||

## 秀丸エディタ独自のソース
|機能|説明|実装|
|:--:|:--:|:--:|
|outline			|アウトラインの一覧||
|project			|プロジェクトのファイル一覧||
|output_panel		|アウトプット枠||


# アクション一覧

## common
|機能|説明|補足|実装済|
|:--:|:--:|:--:|:--:|
|nop					|なにもしない	||○|
|append				|実装しない||×|
|delete				|ファイルの削除|||
|echo				|候補の情報をアウトプット枠へ出力。デバッグ目的で利用する。||○|
|ex					|候補のフルパスをコマンドラインに挿入||×|
|insert				|候補のファイル名を挿入||○|
|insert_directory	|候補のディレクトリ名を挿入||○|
|overwrite			|候補のファイル名を上書き挿入|新規追加||
|overwrite_directory|候補のディレクトリ名を上書き挿入|新規追加||
|preview			|候補をプレビューする||○|
|yank				|候補をクリップボードにコピー||○|
|yank_escape		|候補をクリップボードにコピー(エスケープ<C言語文字列>を行う)|||

## file_base

|機能|説明|補足|実装済|
|:--:|:--:|:--:|:--:|
|open|||○|
|preview|||○|
|mkdir||||
|rename||||
|backup||||
|read||||
|wunix||||
|diff||||
|dirdiff||||
|grep||||
|vimgrep||||
|find||||
|argadd||||
|execute_command||||
|exrename||||

## cdable

|機能|説明|補足|実装済|
|:--:|:--:|:--:|:--:|
|cd||秀丸エディタではカレントディレクトリを移動できないため未対応|×|
|lcd||||
|project_cd||||
|tabnew_cd||||
|tabnew_lcd||||
|narrow||||
|vimshell||アクション名をcommand_promptへ変更。コマンドプロンプトを開くようにした。|△|
|tabvimshell||||
|vimfiler||秀丸ファイラーを開く|△|
|explorer||新アクション。エクスプローラーを開く|△|
|tabvimfiler||||

## file_mru
|機能|説明|補足|実装済|
|:--:|:--:|:--:|:--:|
|remove|ファイル履歴から削除する|新規追加|○|

## window
|機能|説明|補足|実装済|
|:--:|:--:|:--:|:--:|
|preview|ウインドウのプレビューを行う||○|

