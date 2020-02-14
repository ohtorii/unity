# unite.vimとの差分

# このドキュメントの対象者

unite.vimのソースとカインドを作成した方向けのドキュメントです。

# はじめに

Vimと秀丸エディタはそれぞれ思想が異なるテキストエディタなので、unite.vimを秀丸エディタへ素直に移植できません。
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
|file_rec			|カレントディレクトリの全てのファイルの一覧|file_recursiveへ名前を変更|✅|
|file_rec/async		|file_rec の非同期処理版|Unityマクロは非同期処理が基本なのでfile_recursiveへ統合した。|✅|
|find				|外部コマンド "find" の結果を出力|||
|function			|Vim の組み込み関数と unite.vim、neocomplcache で定義されている関数の一覧|||
|grep				|外部コマンド "grep" の結果を出力|ファイルから文字列を見付けるだけの単純なgrepは用意しない。秀丸エディタの高機能なgrepを利用する方法を模索中|❎|
|history/yank		|ヤンクされたワードの一覧|||
|unite-colorscheme	|カラースキームの適用|ソース名をhilightへ変更して実装|✅|
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
|unite-mark|||||


## 秀丸エディタ独自のソース
|機能|説明|秀丸の実装状況|
|:---|:---|:---|
|outline			|アウトラインの一覧|✅|
|project			|プロジェクトのファイル一覧||
|output_panel		|アウトプット枠||


# カインド一覧

## cdable

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|cd				|カレントディレクトリ移動|秀丸エディタではカレントディレクトリを移動できないため未対応|❎|
|lcd			|現在のウィンドウのカレントディレクトリ移動|||
|project_cd		||||
|tabnew_cd		|カレントディレクトリを移動し、新しいタブを開く|||
|tabnew_lcd		||||
|file			|Open this directory by file source.|廃止した。代わりにnarrowを利用します|❎|
|narrow			|Narrow down candidates by the directory name.||✅|
|vimshell		|対象をカレントディレクトリとしてvimshellを起動する|アクション名をcommand_promptへ変更。コマンドプロンプトを開くようにした。|✅|
|tabvimshell	||||
|vimfiler		|対象をカレントディレクトリとしてvimfilerを起動する|秀丸ファイラーを開く|✅|
|🆕explorer		|エクスプローラーを開く|新アクション|✅|
|tabvimfiler	||||


## command

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|edit        |edit command|||
|execute     |execute command||✅|
|grep        |grep this command|||


## common
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|nop				|なにもしない	||✅|
|append				|候補をカーソル右側へ挿入|秀丸エディタ的な操作では無いため実装しない|❎|
|delete				|ファイルの削除|||
|echo				|候補の情報をアウトプット枠へ出力|デバッグ目的で利用する|✅|
|ex					|候補のフルパスをコマンドラインに挿入||❎|
|insert				|候補のファイル名を挿入||✅|
|insert_directory	|候補のディレクトリ名を挿入||✅|
|🆕overwrite		|候補のファイル名を上書き挿入|新規追加|✅|
|🆕overwrite_directory|候補のディレクトリ名を上書き挿入|新規追加|✅|
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
|grep		|ファイルに対してgrep検索|ファイルから文字列を見付けるだけの単純なgrepは用意しない。秀丸エディタの高機能なgrepを利用する方法を模索中|❎|
|grep_directory	|ディレクトリに対してgrep検索|ファイルから文字列を見付けるだけの単純なgrepは用意しない。秀丸エディタの高機能なgrepを利用する方法を模索中|❎|


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
|grep|||❎|
|vimgrep||||
|find||||
|argadd||||
|execute_command||||
|exrename||||
|🆕start||uri.startから移動file_baseへ移動しました|✅|

## file_mru🆕
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|🆕remove|ファイル履歴から削除する|新規追加|✅|

## directory_mru🆕
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

## process

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|sigkill	|send the KILL signal to processes|taskkillコマンドを利用|✅|
|sigterm	|send the TERM signal to processes|||
|sigint		|send the INT signal to processes|||


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
|start|open uri by browser|file_base.startへ移動しました|✅|

## window
|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|preview|ウインドウのプレビューを行う||✅|
|open|open this window buffer|jumpアクションを利用する|❎|
|jump|move to this window|||
|only|only this window|||
|delete|delete windows|closeアクションを利用する|❎|
|close|ウインドウを閉じる|||


# アクション引数

|機能|説明|補足|秀丸の実装状況|
|:---|:---|:---|:---|
|word|候補|GetSelectionText/AppendCandidate関数を利用する|✅|
|action__action||||
|action__args||||
|action__buffer_nr||||
|action__col|カラム位置|SetCandidateActionColumn/GetSelectionActionColumn関数を利用する|✅|
|action__line|行番号|SetCandidateActionLine/GetSelectionActionLine関数を利用する|✅|
|action__col_pattern||||
|action__command|コマンド|SetCandidateActionCommand/GetSelectionActionCommand関数を利用する|✅|
|action__command_args||コマンド引数とは異なる利用方法なので無視した。|❎|
|action__histadd||秀丸エディタにはコマンド履歴が無いため未実装|❎|
|action__complete_info||||
|action__complete_info_lazy||||
|action__complete_pos||||
|action__complete_word||||
|action__description||||
|action__directory|ディレクトリパス名|SetCandidateActionDirectoryName/GetSelectionActionDirectoryName関数を利用する|✅|
|action__filename||kinds\file_vimfiler_base.vimで利用。Unityではaction__pathと同じ扱いにした||
|action__function||||
|action__id||||
|action__mapping||||
|action__path|ファイルパス名|SetCandidateActionFileName/GetSelectionActionFileName関数を利用する|✅|
|action__pattern||||
|action__pid||SetCandidateActionProcessId/GetSelectionActionProcessId関数を利用する||
|action__register||||
|action__regtype||||
|action__signature||||
|action__source_args||||
|action__source_name||||
|action__tab_nr||||
|action__text||||
|action__title||||
|action__type||||
|action__uri||||
|action__window_nr||SetCandidateActionWindowHandle/GetSelectionActionWindowHandle関数を利用する||


# オプション

|オプション|説明|補足|秀丸の実装状況|
|--|--|--|--|
|-no-quit||||
|-no-empty||||
|-no-split||||
|-no-cursor-line||||
|-no-focus||||
|-buffer-name={buffer-name}||||
|-profile-name={profile-name}||||
|-input={input-text}||||
|-path={input-text}||||
|-prompt={prompt-text}||||
|-prompt-visible||||
|-prompt-focus||||
|-prompt-direction="top" or "below"||||
|-candidate-icon={icon-text}||||
|-marked-icon={icon-text}||||
|-no-hide-icon||||
|-default-action={default-action}||||
|-start-insert||||
|-keep-focus||||
|-winwidth={window-width}||||
|-winheight={window-height}||||
|-immediately||||
|-force-immediately||||
|-auto-preview|||✅|
|-auto-highlight|||||
|-complete||||
|-col={column-number}||||
|-vertical||||
|-horizontal||||
|-direction={direction}||||
|-verbose||||
|-auto-resize||||
|-no-resize||||
|-toggle||||
|-quick-match||||
|-create||||
|-update-time={new-update-time}||||
|-abbr-highlight={highlight-name}||||
|-hide-source-names||||
|-max-multi-lines={max-lines}||||
|-here||||
|-silent||||
|-auto-quit||||
|-short-source-names||||
|-multi-line||||
|-resume||||
|-wrap||||
|-select={select-candidate-index}||||
|-log||||
|-truncate||||
|-truncate-width||||
|-tab||||
|-sync||||
|-unique||||
|-cursor-line-time={icon-text}||||
|-wipe||||
|-ignorecase||||
|-smartcase||||
|-no-restore||||
|-vertical-preview||||
|-force-redraw||||
|-previewheight={height}||||
|-no-buffer||||
|-match-input||||
|-file-quit||||
