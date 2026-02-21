#include "main.h"
#include "enemy/node/add_score.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "score.h"
#include "enemy/node/base.h"
#include "imgui.h"
#include <format>

void AddScore::Init(Transform trans)
{
	// ベースデータセット

	// 名前
	SetNameData({ "AddScore", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	// 説明文
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal});

	// ここでSetDescriptionData(GenerateDescriptionText))

	//m_AddScore = 1.0f; // スコア加算量

	// フォントデータ更新
	UpdateDescriptionData();

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init();
	AddInputTypeTop(InputType::Death);
	AddInputTypeBottom(InputType::Death);
	SetCDMax(0);
	SetCD(0);
}

void AddScore::ShowConfigWindow()
{
	ImGui::Begin("AddScore Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("Add Score", &m_AddScore, 0.0f, 100.0f);
	// データを更新したため説明文も更新
	GenerateDescriptionText();
	ImGui::End();
}

bool AddScore::NodeEffect(FieldEnemy* enemy_ptr)
{
	// スコア加算
	Manager::GetCurrentScene()->GetGameObject<Score>()->AddScore(static_cast<int>(m_AddScore));

	return true;
}

// この関数init時とeditにきた瞬間に呼ぶようにしようね
std::string AddScore::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードを通過するとスコアが{}だけ加算されます。";

	// std::formatを使用して最終的な文字列を生成
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_AddScore));

	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	return formatted_text;
}