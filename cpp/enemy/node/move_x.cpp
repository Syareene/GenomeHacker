#include "main.h"
#include "enemy/node/move_x.h"
#include "enemy/field_enemy.h"
#include "imgui.h"

#include <format>

void MoveX::Init(Transform trans)
{
	// ベースデータセット
	m_MoveVal = DEFAULT_MOVE_VAL; // 移動量

	// 名前
	SetNameData({ "MoveX", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal});
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

void MoveX::ShowConfigWindow()
{
	ImGui::Begin("MoveX Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("Move Value", &m_MoveVal, -0.2f, 0.2f);
	// データを更新したため説明文も更新
	GenerateDescriptionText();
	ImGui::End();
}

bool MoveX::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	// 定期的に動かす
	enemy_ptr->AddPosition(Vector3(m_MoveVal, 0.0f, 0.0f)); // x方向に動かす

	return true;
}

std::string MoveX::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードがある敵は毎フレーム{:.2f}だけX軸に対し移動します。";
	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_MoveVal));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}