#pragma once

#include "enemy/node/base.h"

// 多分ui周りのクラス継承?わからんけど
class AddScore: public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetAddScore(const float score) { m_AddScore = score; }
private:
	float m_AddScore = 1.0f; // スコア加算量
};