#pragma once

#include "enemy/node/base.h"
#include <string>

class FieldEnemy;

class MoveCircular : public NodeBase
{
private:
	float m_Radius;           // 円の半径
	int m_Duration;         // 一周にかかる時間(フレーム)

public:
	void Init(Transform trans) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override;
	std::string GenerateDescriptionText();

	// 移動ベクトルを生成する関数
	Vector3 GenerateMovementVector(FieldEnemy* enemy_ptr);

	// パラメータ設定用
	void SetRadius(float radius) { m_Radius = radius; }
	void SetDuration(int duration) { m_Duration = duration; }
};