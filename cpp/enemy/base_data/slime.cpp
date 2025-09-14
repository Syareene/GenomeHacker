// 敵データ基底クラス
// このデータはシーン側かなんかで常に持っといて、ノードの最新適応状態を見れるようにしておく、あとリソースも使い回せるので疑似flyweightとしても使用可能。
// 敵を出す際はこのデータのポインタを渡して出す感じになる。
// ノードに関してはdnaのボタンを押したときにこのクラスからデータを取ってくる形になるかな。

#include "enemy/base_data/slime.h"

#include "texture_manager.h"
#include "manager.h"
#include "object/game_object.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

#include "enemy/node/move_x.h"

void Slime::Register()
{
	// 登録処理

	// そのenemy固有の情報を登録
	SetDnaScreen(std::make_unique<DnaScreenScript>());
	GetDnaScreen()->Init(); // DNAスクリーンの初期化->ここで各種タブの作成が行われる。

	GetDnaScreen()->GetMoveTab()->AddNode<MoveX>(0);

	// テクスチャ生成
	SetTextureID(L"asset\\texture\\slime.png");
	// uvテクスチャの場合はuvのデータも変数に設定する。
	// また、テクスチャに応じて追加でscaleやposの差異を設定
	SetDrawPosDiff({ 0.0f, 0.0f, 0.0f });
	SetDrawScaleDiff({ 2.0f, 2.0f, 2.0f });

	
	// タブが作られたので各ノードに対してenemyの初期ノードを登録しておく。
	
	// ポインタに対して初期ノードを追加する。
	GetDnaScreen()->GetAttackTab()->AddNode<NodeBase>(-1);
}