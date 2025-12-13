#include "main.h"

#include "enemy/enemy_spawner.h"
#include "scene/manager.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"


// enemyDataのhedder
#include "enemy/base_data/slime.h"
#include "enemy/base_data/minotaur.h"

void EnemySpawner::Init()
{
	// 初期化処理
}

void EnemySpawner::Uninit()
{
	// 終了処理
}

void EnemySpawner::Update()
{
	// 更新処理

	// とりあえず一定frame事に敵を出す
	if(m_SpawnTimer > 90)
	{
		m_SpawnTimer = 0;
		// どの敵を出すかはとりあえず固定
		SpawnEnemy<Slime>({0.0f, 1.0f, static_cast<float>(m_SpawnCount) * 0.75f});
		SpawnEnemy<Minotaur>({ 2.0f, 1.0f, static_cast<float>(m_SpawnCount) * 0.75f });
		m_SpawnCount++;
	}

	m_SpawnTimer++;
}