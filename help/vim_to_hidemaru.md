# unite.vimとの差分

## はじめに

Vimと秀丸エディタはそれぞれ思想が異なるテキストエディタなので、unite.vimを素直に秀丸エディタへ移植することはできませんでした。
そこで、unite.vimのソースとカインドを秀丸エディタへどう移植したのか一覧を作りました。


# ソース一覧

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|alias				|g:unite_source_alias_aliases の一覧|||
|bookmark			|ブックマークの一覧（秀丸マクロに命令が存在しない）|||
|buffer				|開かれているバッファの一覧|||
|buffer_tab			|現在のタブで開かれたバッファの一覧|||
|change				|:change の結果を出力|||
|command			|定義されているコマンドの一覧|||
|directory			|カレントディレクトリのディレクトリ一覧||✅|
|directory/new		|ディレクトリの新規作成|||
|directory_mru		|最近開いたディレクトリの一覧||✅|
|file				|カレントディレクトリのファイルとディレクトリ一覧||✅|
|file/new			|ファイルの新規作成|||
|file_mru			|最近開いたファイルの一覧||✅|
|file_point			|ファイル名か URI に一致するデータを出力する|||
|file_rec			|カレントディレクトリの全てのファイルの一覧|||
|file_rec/async		|file_rec の非同期処理版|||
|find				|外部コマンド "find" の結果を出力|||
|function			|Vim の組み込み関数と unite.vim、neocomplcache で定義されている関数の一覧|||
|grep				|外部コマンド "grep" の結果を出力|||
|history/yank		|ヤンクされたワードの一覧|||
|jump				|:jump の結果の出力|||
|jump_point			|file:line に一致するデータを出力|||
|launcher			|$PATH にある実行ファイルの一覧|||
|line				|現在のバッファの行を出力|||
|line/fast			|10000行以上のバッファ時に使用する line|||
|mapping			|定義されているキーマッピングの一覧|||
|menu				|g:unite_source_menu_menus に登録されてあるメニューの一覧|||
|output				|Vim のコマンドの出力結果を出力する（e.g. :mes）|||
|process			|起動しているプロセスの一覧||✅|
|register			|レジスタの一覧|||
|runtimepath		|runtimepath に追加されているパスの一覧|||
|source				|unite-source の一覧|||
|tab				|タブの一覧|||
|vimgrep			|:vimgrep の結果を出力|||
|window				|現在のウィンドウのタブ一覧||✅|
|

## 秀丸エディタ独自のソース
|機能|説明|秀丸の実装状況|
|:---|:---|:---|
|outline			|アウトラインの一覧||
|project			|プロジェクトのファイル一覧||
|output_panel		|アウトプット枠||


# カインド一覧

## cdable

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|cd|カレントディレクトリ移動|秀丸エディタではカレントディレクトリを移動できないため未対応|❎|
|lcd|現在のウィンドウのカレントディレクトリ移動|||
|project_cd||||
|tabnew_cd|カレントディレクトリを移動し、新しいタブを開く|||
|tabnew_lcd||||
|file|Open this directory by file source.||✅|
|narrow|Narrow down candidates by the directory name.||✅|
|vimshell|対象をカレントディレクトリとしてvimshellを起動する|アクション名をcommand_promptへ変更。コマンドプロンプトを開くようにした。|✅|
|tabvimshell||||
|vimfiler|対象をカレントディレクトリとしてvimfilerを起動する|秀丸ファイラーを開く|✅|
|🆕explorer|エクスプローラーを開く|新アクション|✅|
|tabvimfiler||||


## command

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|edit        |edit command|||
|execute     |execute command|||
|grep        |grep this command|||


## common
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|nop					|なにもしない	||✅|
|append				|候補をカーソル右側へ挿入|秀丸エディタ的な操作では無いため実装しない|❎|
|delete				|ファイルの削除|||
|echo				|候補の情報をアウトプット枠へ出力|デバッグ目的で利用する|✅|
|ex					|候補のフルパスをコマンドラインに挿入||❎|
|insert				|候補のファイル名を挿入||✅|
|insert_directory	|候補のディレクトリ名を挿入||✅|
|🆕overwrite			|候補のファイル名を上書き挿入|新規追加||
|🆕overwrite_directory|候補のディレクトリ名を上書き挿入|新規追加||
|preview			|候補をプレビューする|アウトプット枠で表示する|✅|
|yank				|候補をクリップボードにコピー||✅|
|yank_escape		|候補をクリップボードにコピー(エスケープ<C言語文字列>を行う)||✅|


## file

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|open		|ファイルを開く||✅|
|preview	|プレビューウィンドウでファイルを開く|アウトプット枠でファイル先頭を表示する|✅|
|bookmark	|ファイルをブックマークに追加する|||
|mkdir		|ディレクトリ作成(directory_newと同じ感じ？)|||
|diff		|対象ファイルと、もうひとつのdiffを表示|||
|grep		|ファイルに対してgrep検索|||
|grep_directory	|ディレクトリに対してgrep検索|||


## file_base

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|open|||✅|
|preview|||✅|
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

## file_mru
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|🆕remove|ファイル履歴から削除する|新規追加|✅|

## directory_mru
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|🆕remove|フォルダ履歴から削除する|新規追加|✅|

## openable

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|tabopen		|Open the file in a new tab.|||
|choose			|Open the file in a selected window.|||
|split			|Open the file, splitting horizontally.|||
|vsplit			|Open the file, splitting vertically.|||
|left			|Open the file in the left, splitting vertically.|||
|right			|Open the file in the right, splitting vertically.|||
|above			|Open the file in the top, splitting horizontally.|||
|below			|Open the file in the bottom, splitting horizontally.|||
|persist_open	|Open the file in alternate window.  unite window isn't closed.|||
|tabsplit		|Open the files and vsplit in a new tab.|||
|switch			|Open the file in current window or jump to existing window/tabpage.|||
|tabswitch		|Open the file in new tab or jump to existing window/tabpage.|||
|splitswitch	|Open the file in split window or jump to existing window/tabpage.|||
|vsplitswitch	|Open the file in vertical split window or jump to existing window/tabpage.|||


## tab

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|delete      |delete tabs|||
|edit        |rename tabs|||
|open        |open this tab|||
|preview     |preview tab|||
|rename      |rename tabs|||


## uri

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|start|open uri by browser|候補を関連付けで開く|✅|

## window
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|preview|ウインドウのプレビューを行う||✅|

