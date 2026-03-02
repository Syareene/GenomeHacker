#include "main.h"

#include "enemy/base_data/enemy_list.h"


// enemyDataのhedder
#include "enemy/base_data/slime.h"
#include "enemy/base_data/minotaur.h"
#include "enemy/base_data/mage.h"
#include "enemy/base_data/bat.h"
#include "enemy/base_data/ghost.h"
#include "enemy/base_data/skeleton.h"

void EnemyList::Init(const unsigned int& playerId)
{
	// システムタグ追加
	AddTag("system");

	// 敵を追加
	m_EnemyBaseList.emplace_back(std::make_unique<Slime>())->Register(playerId);
	m_EnemyBaseList.emplace_back(std::make_unique<Minotaur>())->Register(playerId);
	m_EnemyBaseList.emplace_back(std::make_unique<Mage>())->Register(playerId);
	m_EnemyBaseList.emplace_back(std::make_unique<Bat>())->Register(playerId);
	m_EnemyBaseList.emplace_back(std::make_unique<Ghost>())->Register(playerId);
	m_EnemyBaseList.emplace_back(std::make_unique<Skeleton>())->Register(playerId);
}

void EnemyList::Uninit()
{
	// リストを開放
	m_EnemyBaseList.clear();
}

void EnemyList::Update()
{
	// 敵パネルの状態を更新するために実行
	for (auto& enemy : m_EnemyBaseList)
	{
		enemy->Update();
	}
}