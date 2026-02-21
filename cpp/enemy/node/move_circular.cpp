#include "main.h"
#include "enemy/node/move_circular.h"
#include "enemy/field_enemy.h"
#include "imgui.h"
#include <format>
#include <cmath>

void MoveCircular::Init(Transform trans)
{
	// ベースデータセット
	SetNameData({ "MoveCircular", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

void MoveCircular::ShowConfigWindow()
{
	ImGui::Begin("MoveCircular Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("Radius", &m_Radius, 0.1f, 10.0f);
	ImGui::SliderInt("Duration", &m_Duration, 30, 600);
	// データを更新したため説明文も更新
	GenerateDescriptionText();
	ImGui::End();
}

bool MoveCircular::NodeEffect(FieldEnemy* enemy_ptr)
{
	// 現在は既存システムに合わせて直接移動を適用
	Vector3 movement = GenerateMovementVector(enemy_ptr);
	enemy_ptr->AddPosition(movement);
	return true;
}

Vector3 MoveCircular::GenerateMovementVector(FieldEnemy* enemy_ptr)
{
	// 生存時間を取得
	unsigned int liveTime = enemy_ptr->GetLiveTime();
	
	// 初回フレーム（生存時間1）では移動しない
	if (liveTime <= 1)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	// 円形移動の進行度を計算
	float progress = static_cast<float>(liveTime % m_Duration) / static_cast<float>(m_Duration);
	float prevProgress = static_cast<float>((liveTime - 1) % m_Duration) / static_cast<float>(m_Duration);

	// 現在と前フレームの円形オフセットを計算
	float currentAngle = progress * 2.0f * 3.14159265f;
	float prevAngle = prevProgress * 2.0f * 3.14159265f;

	Vector3 currentOffset = Vector3(
		m_Radius * cosf(currentAngle),
		0.0f,
		m_Radius * sinf(currentAngle)
	);

	Vector3 prevOffset = Vector3(
		m_Radius * cosf(prevAngle),
		0.0f,
		m_Radius * sinf(prevAngle)
	);

	// 円形移動による差分を計算
	Vector3 circularDelta = currentOffset - prevOffset;

	return circularDelta;
}

std::string MoveCircular::GenerateDescriptionText()
{
	std::string format_string = "このノードがある敵は半径{:.2f}の円上を、{}フレームで移動します。";
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_Radius, m_Duration));
	// メンバに格納
	SetDescriptionData({ formatted_text, Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	return formatted_text;
}