#include "main.h"

#include "enemy/base_data/enemy_list.h"


// enemyDataのhedder
#include "enemy/base_data/slime.h"
#include "enemy/base_data/minotaur.h"

void EnemyList::Init()
{
	// システムタグ追加
	AddTag("system");

	// 敵を追加
	m_EnemyBaseList.emplace_back(std::make_unique<Slime>())->Register();
	m_EnemyBaseList.emplace_back(std::make_unique<Minotaur>())->Register();
}

void EnemyList::Uninit()
{
	// リストを開放
	m_EnemyBaseList.clear();
}

void EnemyList::Update()
{

}