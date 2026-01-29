#include "main.h"
#include "enemy/node_tab/tab_manager.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"


TabManager::TabManager()
{
	// 初期化処理
	m_AttackTab.Init(0);
	m_MoveTab.Init(0);
	m_DeathTab.Init(0);
}

TabManager::~TabManager()
{
	// 終了処理
	//m_AttackTab.Uninit();
	//m_MoveTab.Uninit();
	//m_DeathTab.Uninit();

	// これってメモリ上からデータ消すのどうするんだ？よくよく考えたら。そもそもstd::unique_ptr周りの処理がどうなってるかわからねえから確かめるべきだな
}


// 下記3つの関数は特定のタブを選択状態にするための関数

void TabManager::SelectedAttackTab()
{

}

void TabManager::SelectedMoveTab()
{

}

void TabManager::SelectedDeathTab()
{

}