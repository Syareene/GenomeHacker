#pragma once

#include "scene/state/base_state.h"
#include "player.h"

// 前方宣言
class NodeBase;
class EnemyBase;


class DnaEditState : public State
{
public:
	// DNA編集画面(敵別)用State
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Clicked();
	void ModifyNodePos();
	void ReleaseGrabNode() { m_IsReleaseGrabNode = true; } // 掴んでいるノードを離す
private:
	constexpr static Vector2 NODE_START_POS = { 800.0f, 300.0f }; // ノードの初期配置位置
	bool m_IsReleaseGrabNode = false;
};
