#pragma once

//#include <list>
#include <memory>
#include "object/2d_object.h"
#include "enemy/node/number.h"


// 多分ui周りのクラス継承?わからんけど
class NumberSocket : public Object2D
{
public:
	// これ考えたけどsprite作るのがめんどくさいから形みたいな感じじゃなくて内部処理でくっつくくっつかないとかになるのかな？〇〇の場所で使えるノードみたいな感じで

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	NumberNode* GetNumberNode(); // 自身につけられた数字ノードを取得する
	// setはupdate内でnodeがアタッチされた時に自身で行うので不要
private:
	std::unique_ptr<NumberNode> m_NumberNode = nullptr;
};