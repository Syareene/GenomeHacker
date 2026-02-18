#pragma once

#include "enemy/node/base.h"

class MoveZ : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) 
	{ 
		m_MoveVal = val; 
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}

	std::string GenerateDescriptionText() override; // ノードごとの説明文設定関数

private:
	constexpr static float DEFAULT_MOVE_VAL = 0.02f; // デフォルトの移動量
	float m_MoveVal; // 動く量
};