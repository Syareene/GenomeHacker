#pragma once

#include "enemy/node/base.h"

class MoveX : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) 
	{ 
		m_MoveVal = val;
		// データを更新したため説明文も更新
		UpdateDescriptionData();
	}

private:
	float m_MoveVal = 1.0f; // 動く量
};