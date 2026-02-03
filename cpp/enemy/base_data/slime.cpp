#include "main.h"
#include "enemy/base_data/slime.h"

#include "manager/texture_manager.h"
#include "scene/manager.h"
#include "object/game_object.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

#include "enemy/node/eight_shot.h"
#include "enemy/node/move_x.h"
#include "enemy/node/move_z.h"

#include "enemy/node/add_score.h"
#include "enemy/node/move_circular.h"

EnemyBase* Slime::Register(const unsigned int& playerId)
{
	// 登録処理

	// そのenemy固有の情報を登録
	
	// 初期化処理
	EnemyBase::Init(playerId);
	SetEnemyID(GetEnemyTypeId<Slime>());

	// ノード登録
	// 攻撃タブ
	GetTabManager()->GetAttackTab()->AddNode<EightShot>(0);
	// 移動タブ
	GetTabManager()->GetMoveTab()->AddNode<MoveX>(0);
	GetTabManager()->GetMoveTab()->AddNode<MoveZ>(-1);
	GetTabManager()->GetMoveTab()->AddNode<MoveX>(-1);
	GetTabManager()->GetMoveTab()->AddNode<MoveZ>(-1);
	GetTabManager()->GetMoveTab()->AddNode<MoveX>(-1);
	// 死亡タブ
	GetTabManager()->GetDeathTab()->AddNode<AddScore>(0)->SetAddScore(1);
	GetTabManager()->GetDeathTab()->AddNode<EightShot>(-1);

	// テクスチャ生成
	SetTextureID(L"asset\\texture\\slime.png");
	// uvテクスチャの場合はuvのデータも変数に設定する。
	// また、テクスチャに応じて追加でscaleやposの差異を設定
	SetDrawPosDiff({ 0.0f, 0.0f, 0.0f });
	SetDrawScaleDiff({ 2.0f, 2.0f, 2.0f });

	// 体力設定
	SetMaxHealth(1.0f);

	return this;
}