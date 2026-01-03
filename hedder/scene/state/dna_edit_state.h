#pragma once

#include <list>
#include <memory>
#include <string>
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
	EnemyBase* GetCurrentEnemyBase() const { return m_CurrentEnemyBase; } // 現在編集中の敵データを取得
	void SetCurrentEnemyBase(EnemyBase* enemyBase) { m_CurrentEnemyBase = enemyBase; } // 現在編集中の敵データを設定
	NodeBase* GetGrabbingNode() const { return m_GrabbingNode; } // プレイヤーが現在掴んでいるノードを取得
	void SetGrabbingNode(NodeBase* node) { m_GrabbingNode = node; } // プレイヤーが現在掴んでいるノードを設定
	//Player* GetPlayer() const { return m_Player; }
	//void SetPlayer(Player* player) { m_Player = player; }
private:
	NodeBase* m_GrabbingNode = nullptr; // プレイヤーが現在掴んでいるノード
	EnemyBase* m_CurrentEnemyBase = nullptr; // 現在編集中の敵データ
	//Player* m_Player = nullptr; //->プレイヤーの所持ノードは更新しなくていいから諸々不要そう
	constexpr static Vector2 NODE_START_POS = { 800.0f, 300.0f }; // ノードの初期配置位置

	// プレイヤーのポインタ?
	// ポインタ->GetNodes()->Draw()とかで描画するかな
	// その前に更新時に位置合わせしてupdateやね
};