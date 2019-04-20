// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// または、参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>



// プログラムに必要な追加ヘッダーをここで参照してください
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_set>
#include<unordered_map>
#include<stdio.h>
#include<tchar.h>

#include"define.h"
#include"unity.h"
#include"sources.h"
#include"file.h"
#include"candidates.h"
#include"refine_search.h"

