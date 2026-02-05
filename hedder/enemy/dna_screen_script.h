#pragma once

#include "object/panel.h"
#include "enemy/node_tab/tab_visual.h"

class EnemyBase;

// 見た目部分を管理するクラス
class DnaScreenScript : public Panel
{
public:

	// このクラスがenemy_baseからstateにたされてるので要らなくなったらstateからdestoryすべきかな

	void Init(EnemyBase* base_enemy, const unsigned int& player_id);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// DNAスクリーンの特定の処理を追加
	void ShowDnaInfo();
	void HideDnaInfo();

	// タブ移動する時に掴んでるノードは解放してね!
	inline VisualBase* GetGrabbingNode() const { return m_GrabbingNode; }
	inline void ReleaseGrabbingNode() { m_IsReleaseGrabNode = true; }
	inline void SetGrabbingNode(VisualBase* nodePtr) { m_GrabbingNode = nodePtr; }

	TabVisual* GetActiveTab();
	inline TabVisual* GetAttackTabVisual() { return &m_AttackVisual; }
	inline TabVisual* GetMoveTabVisual() { return &m_MoveVisual; }
	inline TabVisual* GetDeathTabVisual() { return &m_DeathVisual; }

private:
	TabVisual m_AttackVisual;
	TabVisual m_MoveVisual;
	TabVisual m_DeathVisual;

	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();

	// プレイヤーのid(tab_visual用に保存し、playerのノードを見れるようにする)
	unsigned int m_PlayerId = 0;

	VisualBase* m_GrabbingNode = nullptr; // 現在掴んでいるノードのポインタ
	bool m_IsReleaseGrabNode = false; // ノードを離したかどうかのフラグ
	EnemyBase* m_EnemyBase = nullptr; // 参照しているデータ
	static constexpr Vector2 NODE_TAB_TEXT_POS = { 1000.0f, 35.0f }; // ノードタブのテキスト位置
	static constexpr Vector2 PLAYER_NODE_LIST_POS = { 1024.0f, 450.0f }; // プレイヤーノード欄の位置
	static constexpr Vector2 PLAYER_NODE_LIST_SCALE = { 512.0f, 540.0f }; // プレイヤーノード欄のスケール
	static constexpr Vector2 TAB_BUTTON_SIZE = {100.0f, 50.0f};
};