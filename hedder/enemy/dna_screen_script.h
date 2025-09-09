#pragma once

#include "object/panel.h"
#include "enemy/node_tab/tab_base.h"

class AttackTab;
class MoveTab;
class DeathTab;

// ここ仕組み変えないとダメそう

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

	AttackTab* GetAttackTab() { return m_AttackTab; }
	MoveTab* GetMoveTab() { return m_MoveTab; }
	DeathTab* GetDeathTab() { return m_DeathTab; }
private:
	AttackTab* m_AttackTab = nullptr;
	MoveTab* m_MoveTab = nullptr;
	DeathTab* m_DeathTab = nullptr;
};