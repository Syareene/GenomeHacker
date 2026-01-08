#pragma once

#include "object/panel.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

class EnemyBase;

class DnaScreenScript : public Panel
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// DNAスクリーンの特定の処理を追加
	void ShowDnaInfo();
	void HideDnaInfo();

	TabBase* GetActiveTab();

	AttackTab* GetAttackTab() { return m_AttackTab; }
	MoveTab* GetMoveTab() { return m_MoveTab; }
	DeathTab* GetDeathTab() { return m_DeathTab; }
private:
	static constexpr Vector2 TAB_BUTTON_SIZE = {100.0f, 50.0f};
	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();
	AttackTab* m_AttackTab;
	MoveTab* m_MoveTab;
	DeathTab* m_DeathTab;
};