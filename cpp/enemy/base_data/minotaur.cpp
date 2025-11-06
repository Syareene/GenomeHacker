// 敵データ基底クラス
// このデータはシーン側かなんかで常に持っといて、ノードの最新適応状態を見れるようにしておく、あとリソースも使い回せるので疑似flyweightとしても使用可能。
// 敵を出す際はこのデータのポインタを渡して出す感じになる。
// ノードに関してはdnaのボタンを押したときにこのクラスからデータを取ってくる形になるかな。
#include "main.h"

#include "enemy/base_data/minotaur.h"

#include "manager/texture_manager.h"
#include "scene/manager.h"
#include "object/game_object.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

#include "enemy/node/move_x.h"
#include "enemy/node/add_score.h"

void Minotaur::Register()
{
	// 登録処理

	// そのenemy固有の情報を登録
	SetDnaScreen(std::make_unique<DnaScreenScript>());
	GetDnaScreen()->Init(); // DNAスクリーンの初期化->ここで各種タブの作成が行われる。

	GetDnaScreen()->GetMoveTab()->AddNode<MoveX>(0)->SetMoveVal(0.05f);

	GetDnaScreen()->GetDeathTab()->AddNode<AddScore>(0)->SetAddScore(10);

	// テクスチャ生成
	SetTextureID(L"asset\\texture\\minotaur.png");
	// uvテクスチャの場合はuvのデータも変数に設定する。
	// また、テクスチャに応じて追加でscaleやposの差異を設定
	SetDrawPosDiff({ 0.0f, 0.0f, 0.0f });
	SetDrawScaleDiff({ 1.5f, 1.5f, 1.5f });

	// 体力設定
	SetMaxHealth(3.0f);
}