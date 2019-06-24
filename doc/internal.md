# 内部実装について

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
