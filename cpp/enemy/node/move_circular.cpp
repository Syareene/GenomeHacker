#include "main.h"
#include "enemy/node/move_circular.h"
#include "enemy/field_enemy.h"
#include <format>
#include <cmath>

void MoveCircular::Init(Transform trans)
{
	Transform defaultTrans = Transform();
	defaultTrans.SetScale(Vector3(500.0f, 100.0f, 0.0f));
	defaultTrans.SetPosition(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// ベースデータセット
	SetNameData({ "MoveCircular", Vector2(10.0f, 10.0f), NodeBase::TextType::Normal });
	SetDescriptionData({ GenerateDescriptionText(), Vector2(10.0f, 350.0f), NodeBase::TextType::Normal });

	// フォント作られてから基底クラスのinitを呼ぶ
	NodeBase::Init(defaultTrans);
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);

	// 円形移動パラメータの初期化
	m_Radius = 2.0f;
	m_Duration = 3.0f;
	m_CurrentTime = 0.0f;
	m_StartPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_IsInitialized = false;
	m_LastCircularPosition = Vector3(0.0f, 0.0f, 0.0f);
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
	// 初回実行時の初期化
	if (!m_IsInitialized)
	{
		m_StartPosition = enemy_ptr->GetPosition();
		m_IsInitialized = true;
		m_CurrentTime = 0.0f;

		// 初期円形位置を計算（半径分右にずれた位置から開始）
		m_LastCircularPosition = Vector3(m_Radius, 0.0f, 0.0f);
		return Vector3(0.0f, 0.0f, 0.0f); // 初回は移動しない
	}

	// 時間更新（60FPS想定）
	float deltaTime = 1.0f / 60.0f;
	m_CurrentTime += deltaTime;

	// 一周したらリセット
	if (m_CurrentTime >= m_Duration)
	{
		m_CurrentTime = 0.0f;
	}

	// 現在の円形座標を計算
	float angle = (m_CurrentTime / m_Duration) * 2.0f * 3.14159265f;
	Vector3 currentCircularPosition = Vector3(
		m_Radius * cosf(angle),
		0.0f,
		m_Radius * sinf(angle)
	);

	// 円形移動の差分ベクトルを計算
	Vector3 circularMovement = currentCircularPosition - m_LastCircularPosition;

	// 次フレームのために保存
	m_LastCircularPosition = currentCircularPosition;

	return circularMovement;
}

std::string MoveCircular::GenerateDescriptionText()
{
	std::string format_string = "このノードがある敵は半径{}、{}秒で円形に移動します。他の移動ノードと組み合わせ可能です。";
	std::string formatted_text = std::vformat(format_string, std::make_format_args(m_Radius, m_Duration));
	return formatted_text;
}