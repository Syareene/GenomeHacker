#pragma once

#include "enemy/node/base.h"
#include <string>

class FieldEnemy;

class MoveCircular : public NodeBase
{
private:
	float m_Radius;           // 円の半径
	float m_Duration;         // 一周にかかる時間（秒）
	float m_CurrentTime;      // 現在の経過時間
	Vector3 m_StartPosition;  // 開始座標
	bool m_IsInitialized;     // 初期化フラグ
	Vector3 m_LastCircularPosition; // 前フレームの円形座標

public:
	void Init(Transform trans) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override;
	std::string GenerateDescriptionText();

	// 新しい設計：移動ベクトルを生成する関数
	Vector3 GenerateMovementVector(FieldEnemy* enemy_ptr);

	// パラメータ設定用
	void SetRadius(float radius) { m_Radius = radius; }
	void SetDuration(float duration) { m_Duration = duration; }
};