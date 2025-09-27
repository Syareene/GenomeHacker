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

	// このタイミングで敵の元データを全て登録
	// 自動でしたいけど一旦手動かな～ファイル読み込みとかしない限り自動化できん

	// ->enemy_listに移行済み
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


// どのenemyを生成するかの指定方法をどうするかだよね。
// indexからかな？とはいえwaveとかでやるなら話が異なってくる

/*
void EnemySpawner::SetEnemyData(FieldEnemy* set_target, int target_id)
{
	// ポインタを受け取って特定の敵の初期化情報をセットする関数

	// 受け取った引数のポインタの型を確認し、変数に保存されている敵の元データを参照しセットする
	for(const auto& base : m_EnemyBaseList)
	{
		if (target_id == base.get()->GetEnemyID())
		{
			// 一致したのでデータをセットする

			// fieldenemyに欲しいデータ上げておく
			// base_dataへのリファレンス(不動及びnodeデータはここから引っ張ってくる)
			// 後は変動するステータス: 現在HP

			set_target->SetEnemyBase(base.get());
			set_target->SetCurrentHP(base.get()->GetMaxHealth());

			// スポーン時の座標だけセット
			// んー、変動するだろうからほんちゃんは外のほうがいいかも
			set_target->SetPosition({ 0.0f, 0.0f, 0.0f });

			// データセットしたのでループから抜ける
			break;
		}
	}
}
*/