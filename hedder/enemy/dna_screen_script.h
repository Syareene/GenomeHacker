#pragma once

#include "object/panel.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

class EnemyBase;

class DnaScreenScript : public Panel
{
public:
	struct TabList
	{
		AttackTab* attackTab = nullptr;
		MoveTab* moveTab = nullptr;
		DeathTab* deathTab = nullptr;
	};

	DnaScreenScript::TabList Init(const unsigned int& playerId, Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// DNAスクリーンの特定の処理を追加
	void ShowDnaInfo();
	void HideDnaInfo();

	TabBase* GetActiveTab();

	unsigned int GetAttackTabId() const { return m_AttackTabId; }
	unsigned int GetMoveTabId() const { return m_MoveTabId; }
	unsigned int GetDeathTabId() const { return m_DeathTabId; }
private:
	static constexpr Vector2 TAB_BUTTON_SIZE = {100.0f, 50.0f};
	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();
	unsigned int m_AttackTabId;
	unsigned int m_MoveTabId;
	unsigned int m_DeathTabId;
};