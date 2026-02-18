#pragma once

#include "enemy/node/base.h"

class Area : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetDurationVal(const float val) 
	{ 
		m_Duration = val;
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}
	std::string GenerateDescriptionText() override; // ノードごとの説明文設定関数

private:
	float m_Duration = 150.0f; // エリアの持続時間(フレーム数)
	float m_ShotInterval = 300.0f; // 発射間隔(フレーム数)
	float m_AreaDamage = 1.0f; // エリアのダメージ(仮)
};