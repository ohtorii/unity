// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// または、参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define NOMINMAX
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>



// プログラムに必要な追加ヘッダーをここで参照してください

/////////////////////////////////////////////////////////////////////////////
//3rd-party
/////////////////////////////////////////////////////////////////////////////

//cereal
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

//libguess
#include<libguess/libguess.h>



/////////////////////////////////////////////////////////////////////////////
//unity
/////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include<vector>
#include<deque>
#include<string>
#include<algorithm>
#include<unordered_set>
#include<unordered_map>
#include<stdio.h>
#include<chrono>
#include<tchar.h>

#include"parallel.h"
#include"log.h"
#include"unicode_helper.h"
#include"define.h"
#include"unity.h"
#include"sources.h"
#include"file.h"
#include"candidates.h"
#include"refine_search.h"
#include"inheritance.h"
#include"utility.h"
#include"interface_sugar.h"
#include"static_status.h"
#include"context_status.h"
#include"user_data.h"
#include"async_files.h"
#include"recurring_task.h"
#include"auto_preview.h"

