#include "main.h"
#include "enemy/node/move_circular.h"
#include "enemy/field_enemy.h"
#include <format>
#include <cmath>

void MoveCircular::Init(Transform trans)
{
	//Transform defaultTrans = Transform();
	//defaultTrans.SetScale(Vector3(500.0f, 100.0f, 0.0f));
	//defaultTrans.SetPosition(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// ベースデータセット
	SetNameData({ "MoveCircular", Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });
	SetDescriptionData({ GenerateDescriptionText(), Vector2(0.0f, 0.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ
	NodeBase::Init();
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);

	// 円形移動パラメータの初期化
	m_Radius = 3.0f;
	m_Duration = 180;
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
	std::string format_string = "このノードがある敵は半径{}、{}フレームで円形に移動します。他の移動ノードと組み合わせ可能です。";
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_Radius, m_Duration));
	return formatted_text;
}