#include "main.h"

#include "enemy/enemy_spawner.h"
#include "scene/manager.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "enemy/base_data/enemy_list.h"
#include "lib/random_number.h"


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

	// 出現間隔が0以下なら敵を出す準備をする
	if (m_TimeNextWave <= 0)
	{
		// 出す敵を決定
		int enemy_count = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBaseCount();
		// ランダム
		int enemy_index = RandomNumber::GetInstance()->GetRandomInt(0, enemy_count - 1);
		// 出現する敵の型情報保存しておく
		auto it = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases().begin();
		std::advance(it, enemy_index);
		m_EnemyBaseData = it->get();

		// コストを元に出現数と間隔を決定

		// 出現数(とりあえず固定)
		m_SpawnCount = 10;
		// 敵が出現する事の間隔とウェーブ間隔は1:2で一旦設定
		int between_time = m_TimeCost / 3;
		m_EnemySpawnInterval = between_time / m_SpawnCount;
		m_SpawnTimer = 0;
		m_TimeNextWave = between_time * 2;

		// コストを増加させる(とりあえず固定値)
		m_TimeCost -= 1;
		m_EnemyCost += 1;
	}

	// SpawnCountが1以上なら敵を出す処理
	if (m_SpawnCount > 0)
	{
		// spawntimerを減らす
		m_SpawnTimer--;

		// m_SpawnTimerが0以下なら敵を出す
		if (m_SpawnTimer <= 0)
		{
			// 敵出現処理
			Vector3 spawn_pos = { RandomNumber::GetInstance()->GetRandomFloat(-5.0f, 5.0f), 1.0f, RandomNumber::GetInstance()->GetRandomFloat(-5.0f, 5.0f) };
			SpawnEnemyByData(spawn_pos);

			// 出現後諸々をセットし直す
			m_SpawnedCount++;
			m_SpawnCount--;
			m_SpawnTimer = m_EnemySpawnInterval;
		}
	}
	// SpawnCountが0以下ならTimeNextWaveを減らす
	else
	{
		m_TimeNextWave--;
	}


	// とりあえず一定frame事に敵を出す
	//if(m_SpawnTimer > 90)
	//{
	//	m_SpawnTimer = 0;
	//	// どの敵を出すかはとりあえず固定
	//	SpawnEnemy<Slime>({0.0f, 1.0f, static_cast<float>(m_SpawnedCount) * 0.75f});
	//	SpawnEnemy<Minotaur>({ 2.0f, 1.0f, static_cast<float>(m_SpawnedCount) * 0.75f });
	//	m_SpawnedCount++;
	//}

	//m_SpawnTimer++;
}