# 内部実装について

このドキュメントはUnityマクロの内部実装について書いています。


# sourceについて

## candidate_type

候補を返す方法を指定する
|値|説明|
|:--:|:--:|
|function|DLL側の関数を利用して候補を作成する|
|list|候補を改行区切りのリストで返す|


# kindについて

## is_quit
アクション実行後に終了するかどうか。
ディフォルト値	false

## is_multi_selectable
複数選択に対応したアクションかどうか
ディフォルト値	false

## is_start
ソースを生成してアクションを開始するかどうか
ディフォルト値 false

# 固有名詞
## 秀丸エディタ
|固有名詞|説明|備考|
|:--:|:--:|:--:|
|行番号|1開始の値||
|行インデックス|0開始の値|行インデックス=行番号-1|
 
 	
# 静的変数
## $unity.target_hidemaruhandle (int)
文字列などを挿入する操作対象となる秀丸ハンドル

## $unity.root_macro_directory (string)
マクロのルートディレクトリ

## $unity.is_quit (bool)
処理を終了するかどうか


## $unity.hidemaruhandle_to_focus_at_end_of_process (int)
unity終了後にフォーカスするウインドウ
0 = 無効なハンドル


# アクションの上書きについて

|種類|優先度|
|:--:|:--:|
|kind|低|
|source|高|

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

