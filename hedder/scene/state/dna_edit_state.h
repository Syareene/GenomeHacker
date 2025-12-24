#pragma once

#include <list>
#include <memory>
#include <string>
#include "scene/state/base_state.h"

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
	EnemyBase* GetCurrentEnemyBase() const { return m_CurrentEnemyBase; } // 現在編集中の敵データを取得
	void SetCurrentEnemyBase(EnemyBase* enemyBase) { m_CurrentEnemyBase = enemyBase; } // 現在編集中の敵データを設定
	NodeBase* GetGrabbingNode() const { return m_GrabbingNode; } // プレイヤーが現在掴んでいるノードを取得
	void SetGrabbingNode(NodeBase* node) { m_GrabbingNode = node; } // プレイヤーが現在掴んでいるノードを設定
private:
	NodeBase* m_GrabbingNode = nullptr; // プレイヤーが現在掴んでいるノード
	EnemyBase* m_CurrentEnemyBase = nullptr; // 現在編集中の敵データ
};