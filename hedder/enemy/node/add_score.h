#pragma once

#include "enemy/node/base.h"

class AddScore: public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void ShowConfigWindow() override; // ノード固有のパラメータをセットするデバッグ用ウィンドウを出す関数
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetAddScore(const float score) 
	{ 
		m_AddScore = score;
		// データを更新したため説明文も更新
		GenerateDescriptionText();
	}
	std::string GenerateDescriptionText() override; // ノードごとの説明文設定関数

private:
	float m_AddScore = 1.0f; // スコア加算量
};