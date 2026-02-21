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
	void ShowConfigWindow() override; // ノード固有のパラメータをセットするデバッグ用ウィンドウを出す関数
	bool NodeEffect(FieldEnemy* enemy_ptr) override;
	std::string GenerateDescriptionText() override; // ノードごとの説明文設定関数
private:
};