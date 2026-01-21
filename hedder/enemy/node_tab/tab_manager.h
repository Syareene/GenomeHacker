#pragma once

#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

// 前方宣言
class TabBase;


// EnemyBaseにはこっち側を保存することにする
class TabManager
{
public:
	// オブジェクトではないのでコンストラクタ/デストラクタで管理することとする
	TabManager();
	~TabManager();

	//TabBase* GetActiveTab();
	AttackTab* GetAttackTab() { return &m_AttackTab; }
	MoveTab* GetMoveTab() { return &m_MoveTab; }
	DeathTab* GetDeathTab() { return &m_DeathTab; }
	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();
private:
	AttackTab m_AttackTab;
	MoveTab m_MoveTab;
	DeathTab m_DeathTab;
};