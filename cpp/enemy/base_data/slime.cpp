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
#include "enemy/node/move_to_player.h"

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
	//GetTabManager()->GetAttackTab()->AddNode<EightShot>(0);
	// 移動タブ
	GetTabManager()->GetMoveTab()->AddNode<MoveToPlayer>(0);
	// 死亡タブ
	GetTabManager()->GetDeathTab()->AddNode<AddScore>(0)->SetAddScore(SCORE);
	GetTabManager()->GetDeathTab()->AddNode<EightShot>(-1);

	// テクスチャ生成
	SetTextureID(L"asset\\texture\\monsters.png");
	// uvテクスチャの場合はuvのデータも変数に設定する。
	SetTextureSplitCount(DEFAULT_TEXTURE_COUNT);
	SetUVPos(Vector2(1.0f, 2.0f));
	// また、テクスチャに応じて追加でscaleやposの差異を設定
	SetDrawPosDiff(DRAW_POS_DIFF);
	SetDrawScaleDiff(DRAW_SCALE_DIFF);

	// 体力設定
	SetMaxHealth(HEALTH);

	return this;
}