#pragma once

#include <list>
#include <memory>
#include <string>
#include "scene/state/base_state.h"

class NodeBase; // 前方宣言


class DnaEditState : public State
{
public:
	// DNA編集画面(敵別)用State
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	NodeBase* GetGrabbingNode() const { return m_GrabbingNode; } // プレイヤーが現在掴んでいるノードを取得
private:
	NodeBase* m_GrabbingNode = nullptr; // プレイヤーが現在掴んでいるノード
};