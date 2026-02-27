#include "main.h"
#include "enemy/node/move_to_player.h"
#include "enemy/field_enemy.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "player.h"
#include "imgui.h"

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

void MoveToPlayer::ShowConfigWindow()
{
	// 上限値と下限値に関しては移動速度は遅すぎず早すぎずの範囲で、
	// 発射間隔とかもそんな感じで調節しておけばいいかなと。

	// NodeでのWindow設定適応
	ImWindowSettings();
	// ウィンドウ生成
	ImGui::Begin("MoveToPlayer Config");
	ImGui::SeparatorText("Properties");
	// 設定可能なパラメーターを列挙
	if (ImGui::SliderFloat("Move Value", &m_MoveVal, 0.01f, 0.3f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		// データを更新したため説明文も更新
		SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	}
	// どのタブで使えるかを表示
	ShowTabInfo();

	ImGui::End();
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

	// プレイヤーとの距離が1.25以下なら移動しない
	if(to_player.length() <= 1.25f)
	{
		return true;
	}

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