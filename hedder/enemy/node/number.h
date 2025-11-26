#pragma once

//#include <list>
#include "enemy/node/base.h"
#include "enemy/field_enemy.h"

// 多分ui周りのクラス継承?わからんけど
class NumberNode : public NodeBase
{
public:
	// これ考えたけどsprite作るのがめんどくさいから形みたいな感じじゃなくて内部処理でくっつくくっつかないとかになるのかな？〇〇の場所で使えるノードみたいな感じで

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override;
};