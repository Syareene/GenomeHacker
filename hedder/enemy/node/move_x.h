#pragma once

#include "enemy/node/base.h"

// 多分ui周りのクラス継承?わからんけど
class MoveX : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) { m_MoveVal = val; }
private:
	float m_MoveVal = 1.0f; // 動く量
};