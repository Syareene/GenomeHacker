#include "main.h"
#include "enemy/node/move_z.h"
#include "enemy/field_enemy.h"

void MoveZ::Init(Transform trans)
{
	Transform defaultTrans = Transform();
	defaultTrans.SetScale(Vector3(500.0f, 100.0f, 0.0f));
	defaultTrans.SetPosition(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// ベースデータセット

	// 名前
	SetNameData({ "MoveZ", Vector2(10.0f, 10.0f), NodeBase::TextType::Normal });
	// 説明文
	AddDescriptionData({ "このノードがある敵は毎フレームn分だけZ軸に対し移動します。", Vector2(10.0f, 350.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init(defaultTrans);
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
	m_MoveVal = 0.02f; // 移動量
}

bool MoveZ::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	// 定期的に動かす
	enemy_ptr->AddPosition(Vector3(0.0f, 0.0f, m_MoveVal)); // z方向に動かす

	return true;
}