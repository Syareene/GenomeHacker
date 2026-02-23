#include "main.h"
#include "enemy/node/move_z.h"
#include "enemy/field_enemy.h"
#include "imgui.h"

#include <format>

void MoveZ::Init(Transform trans)
{
	// ベースデータセット
	m_MoveVal = DEFAULT_MOVE_VAL; // 移動量

	// 名前
	SetNameData({ "MoveZ", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal});

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

void MoveZ::ShowConfigWindow()
{
	// NodeでのWindow設定適応
	ImWindowSettings();
	// ウィンドウ生成
	ImGui::Begin("MoveZ Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	// 設定可能なパラメーターを列挙
	if (ImGui::SliderFloat("Move Value", &m_MoveVal, -0.3f, 0.3f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}

	ImGui::End();
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

std::string MoveZ::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は毎フレーム{:.2f}だけZ軸に対し移動します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_MoveVal));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}