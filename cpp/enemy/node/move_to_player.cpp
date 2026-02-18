#include "main.h"
#include "enemy/node/move_to_player.h"
#include "enemy/field_enemy.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "player.h"

#include <format>

void MoveToPlayer::Init(Transform trans)
{
	// ベースデータセット
	m_MoveVal = DEFAULT_MOVE_VAL; // 移動量

	// 名前
	SetNameData({ "MoveToPlayer", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal});
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

bool MoveToPlayer::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	Vector3 player_pos = Manager::GetCurrentScene()->GetGameObject<Player>()->GetPosition();
	Vector3 enemy_pos = enemy_ptr->GetPosition();
	// playerに向かうベクトルを計算
	Vector3 to_player = player_pos - enemy_pos;
	to_player.normalize();
	// 移動量を掛ける
	to_player *= m_MoveVal;
	// 動かす
	enemy_ptr->AddPosition(to_player);

	return true;
}

std::string MoveToPlayer::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は毎フレーム{:.2f}だけ敵に向かって移動します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_MoveVal));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}