# ソースとカインドの作成方法

- [はじめに](#はじめに)
- [ソースのサンプルコード](#ソースのサンプルコード)
  - [候補の取得方法](#候補の取得方法)
  - [実際のコード](#実際のコード)
- [カインドのサンプルコード](#カインドのサンプルコード)
  - [カインドの継承](#カインドの継承)
- [連絡先](#連絡先)


# はじめに

このドキュメントではユーザー定義のソースとカインドを作成する方法を説明します。

# ソースのサンプルコード

以下ファイルをもとに改造するのが分かりやすいです。

|候補の取得方法|定義ファイル(.ini)|マクロ処理(.mac)|
|--|--|--|
|文字列|[sources\emoji.ini](../sources/emoji.ini)|[sources\emoji.mac](../sources/emoji.mac)|
|関数|[sources\file_mru.ini](../sources/file_mru.ini)|[sources\file_mru.mac](../sources/file_mru.mac)|
|非同期ファイル|[sources\file_recursive.ini](../sources/file_recursive.ini)|[sources\file_recursive.mac](../sources/file_recursive.mac)|



## 候補の取得方法

候補の取得方法は用途に合わせて、以下３種類あります。

- 文字列(string)
- 関数(function)
- 非同期ファイル(async_file)

それぞれ以下の特徴があります

|候補の取得方法|説明|容易さ|カスタマイズ|非同期|
|--|--|--|--|--|
|文字列|候補を文字列で設定します|○|×|×|
|関数|候補を関数で設定します、候補を細かくカスタマイズできます|×|○|×|
|非同期ファイル|全候補を取得するのに時間がかかる場合に利用します。|×|×|○|


## 実際のコード

候補を集める重要な箇所だけかいつまんで載せています、実際の記述はそれぞれのファイルを参照してください。

### 補足

- 秀丸マクロは９０年代に作られたマクロなので現代とは大分雰囲気が異なります。
- *dllfuncw命令*はDLLを呼び出す秀丸マクロの命令です、


### 固定リスト

emoji.ini

	[property]
	description=絵文字
	default_kind=common
	;候補を文字列で返す
	candidate_type=string


emoji.mac

	//候補をタブ(\t)区切りの文字列で返します
	gather_candidates:
		return @"😀	@description	grinning face	U+1F600
			😃	@description	grinning face with big eyes	U+1F603
			😄	@description	grinning face with smiling eyes	U+1F604
			😁	@description	beaming face with smiling eyes	U+1F601
			😆	@description	grinning squinting face	U+1F606
			😅	@description	grinning face with sweat	U+1F605
			🤣	@description	rolling on the floor laughing	U+1F923";

@descriptionは[internal.md -> 候補のフォーマットについて](./internal.md#候補のフォーマットについて)を参照してください。

### 関数

file_mru.ini

	[property]
	description=ファイル履歴
	default_kind=file_mru
	;候補を関数で設定する
	candidate_type=function


file_mru.mac

	gather_candidates:
		/*候補を関数で設定する
		
		返値	"1"		成功
				"0"		失敗
		*/
		
		//ファイル履歴の個数(filehistcount)だけループを回す
		##i=0;
		while(##i < filehistcount){
			//候補の文字列を追加する
			$$file_name=getfilehist(##i);
			##success=dllfuncw(#g_dll_ohtorii_tools, "AppendCandidate", $$file_name,"");
			if(! ##success){
				return "0";
			}
			
			//候補のファイル名を設定する
			##success=dllfuncw(#g_dll_ohtorii_tools, "SetCandidateActionFileName", $$file_name);
			if(! ##success){
				return "0";
			}
			
			//ファイル履歴の削除時に、ファイル履歴のインデックスが必要となるので、
			//ファイル履歴のインデックス(##i)をユーザーデータで覚えておく。
			##success = dllfuncw(#g_dll_ohtorii_tools,"SetCandidateUserDataNumeric","file_history_index",##i);
			if(! ##success){
				return "0";
			}
			##i = ##i + 1;
		}
		return "1";


### 非同期ファイル

file_recursive.ini

	[property]
	description=カレントディレクトリ以下のファイル一覧（再帰）
	default_kind=file
	;候補を非同期ファイルで返す
	candidate_type=async_file


file_recursive.mac

	gather_candidates:
		$$root_dir=$$1;
		if($$root_dir==""){
			//カレントディレクトリを使用する
			$$root_dir=".";
		}
		
		/*候補を非同期で追記する一時ファイル名を作る。
		*/
		execmacro $g_root_macro_directory+"\\internal\\create_temp_file.mac", "filerec", ".tsv";
		$$temp_filename=getresultex(-1);
		if($$temp_filename==""){
			return "";
		}
		
		/*一時ファイルをUnityマクロ終了時に削除するため、一時ファイル名を登録する。
		*/
		##ret=dllfuncw(#g_dll_ohtorii_tools,"FileRegistAfterDeleteFile",$$temp_filename);
		if(! ##ret){
			return "";
		}

		/* memo
		\で終端しているディレクトリ名をダブルクォーテーションで囲ってコマンドライン引数で渡すときの対策。
		*/
		call remove_terminate $$root_dir;
		$$root_dir=$$return;
		
		/* file_searcher.exe がファイルを探して、一時ファイルへ候補を追記します。
		*/
		$$exe=$g_root_macro_directory+R"(\internal\bin\file_searcher.exe)";
		
		/* file_searcher.exeのコマンドライン引数を作ります
		*/
		$$args = sprintf(R"(--output "%s" --mode file --working_directory "%s" --path "%s" --delay %d)",$$temp_filename,$g_current_working_directory,$$root_dir,0);
		
		/* file_searcher.exe を実行します。
		*/
		##handle=dllfuncw(#g_dll_hm_process,"SpawnWithRedirect",$$exe,$$args,true,true);
		##ret=dllfuncw(#g_dll_hm_process,"SetCreateNoWindow",##handle,true);
		##ret=dllfuncw(#g_dll_hm_process,"Start",##handle);
		if(##ret==0){
			return "";
		}
		
		/*候補が追記される、一時ファイル名を返します。
		*/
		return $$temp_filename;


# カインドのサンプルコード

以下ファイルをもとに改造するのが分かりやすいです。

- [（定義ファイル）kinds\common.ini](../kinds/common.ini)
- [（マクロ処理）  kinds\common.mac](../kinds/common.mac)

echoアクションは、選択した候補の情報を全てダンプ出力します。
データへのアクセス方法の参考にしてください。

common.ini（echoアクション定義を抜粋）

	[property]
	description=共通アクション
	default_action=insert
	'継承するアクション（空白区切り）
	base_kind=

	[action.echo]
	function=echo
	is_multi_selectable=true
	description=候補のデバッグ情報をアウトプット枠へ出力する

common.mac（echoアクションのソースコードを抜粋）

	echo:
		/* 候補の詳細をアウトプット枠へ表示する(debug用途)
		*/
		##output_dll=loaddll("HmOutputPane.dll");
		if(!result){
			return "0";
		}

		//アウトプット枠クリア
		##h=dllfunc(##output_dll,"GetWindowHandle",hidemaruhandle(0));
		##ret=sendmessage(##h,0x111/*WM_COMMAND*/,1009,0);//1009=クリア
	    
	    $$format = R"([%d/%d] {"text":"%s", "source_name":"%s", "action_directory_name":"%s", "action_file_name":"%s", "action_command":"%s", "action_window_handle":%d, "action_line":%d, "action_column":%d, "description":"%s"})" + "\r\n";
	    
		//選択された候補数(##count)を取得する
		##count=dllfuncw(#g_dll_ohtorii_tools,"GetSelectionCount");

		##i    =0;
		while(##i < ##count){
	        //候補の情報を取得する
	        $$source_name           =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionSourceName"         , ##i);
	        $$text                  =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionText"               , ##i);
	        $$action_directory_name =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionActionDirectoryName", ##i);
	        $$action_file_name      =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionActionFileName"     , ##i);
	        $$action_command        =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionActionCommand"      , ##i);
	        ##window_handle         =dllfuncw(   #g_dll_ohtorii_tools, "GetSelectionActionWindowHandle" , ##i);
	        ##line                  =dllfuncw(   #g_dll_ohtorii_tools, "GetSelectionActionLine"         , ##i);
	        ##column                =dllfuncw(   #g_dll_ohtorii_tools, "GetSelectionActionColumn"       , ##i);
	        $$description           =dllfuncstrw(#g_dll_ohtorii_tools, "GetSelectionDescription"        , ##i);
	        
	        //候補の情報を文字列($$info)にする
	        $$info=sprintf($$format
	                , ##i + 1
	                , ##count
	                , $$text
	                , $$source_name
	                , $$action_directory_name
	                , $$action_file_name
	                , $$action_command
	                , ##window_handle
	                , ##line
	                , ##column
	                , $$description);
	        //アウトプット枠へ$$infoを表示する
	        ##ret=dllfunc(##output_dll,"Output",hidemaruhandle(0),$$info);

	        ##i = ##i + 1;
	    }
	    freedll ##output_dll;
	    return "1";

## カインドの継承

カインドの継承は以下定義ファイルを参照してください。

[kinds/file.ini](../kinds/file.ini)

file.ini

	[property]
	description=ファイルのアクション
	default_action=open
	; 継承するカインドを空白区切りで並べます
	base_kind=file_base cdable common


# 連絡先

- <https://ohtorii.hatenadiary.jp>
- <https://twitter.com/ohtorii>
- <https://github.com/ohtorii>
