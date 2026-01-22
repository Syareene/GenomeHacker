#pragma once

#include "enemy/node/base.h"

class AddScore: public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetAddScore(const float score) 
	{ 
		m_AddScore = score;
		// データを更新したため説明文も更新
		UpdateDescriptionData();
	}

private:
	float m_AddScore = 1.0f; // スコア加算量
};