#pragma once

#include "object/panel.h"
#include "enemy/node_tab/tab_visual.h"

class EnemyBase;

// 各種タブのデータを保存するクラスとして変更(名称も変えようかな)
class DnaScreenScript : public Panel
{
public:
	static constexpr size_t MAX_OBJECTS = 2; // オブジェクトvector最大数。継承先クラスで変更可能。

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
	inline void ReleaseGrabbingNode() { m_GrabbingNode = nullptr; }
	inline void SetGrabbingNode(VisualBase* nodePtr) { m_GrabbingNode = nodePtr; }

	TabVisual* GetActiveTab();
	inline TabVisual* GetAttackTabVisual() { return &m_AttackVisual; }
	inline TabVisual* GetMoveTabVisual() { return &m_MoveVisual; }
	inline TabVisual* GetDeathTabVisual() { return &m_DeathVisual; }

private:
	// 実体or unique
	TabVisual m_AttackVisual;
	TabVisual m_MoveVisual;
	TabVisual m_DeathVisual;

	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();

	// プレイヤーのid(tab_visual用に保存し、playerのノードを見れるようにする)
	unsigned int m_PlayerId = 0;

	VisualBase* m_GrabbingNode = nullptr; // 現在掴んでいるノードのポインタ
	EnemyBase* m_EnemyBase = nullptr; // 参照しているデータ
	static constexpr Vector2 TAB_BUTTON_SIZE = {100.0f, 50.0f};
};