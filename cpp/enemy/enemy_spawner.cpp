#include "enemy/enemy_spawner.h"
#include "manager.h"
#include "enemy/field_enemy.h"

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
	SpawnEnemy();
}

void EnemySpawner::SpawnEnemy()
{
	// 敵を出す処理

	// ここのtemplateに指定するのはenemy_baseを継承したクラスではなく、field_enemyを継承したクラス?->しかしfield_enemyは派生クラスなしであくまでデータを受け取って適応させる型にしたい。
	// でも今のaddgameobjectの仕様だと型から実体化しかできないので、enemy_baseの敵情報を何処かに保存し、
	// そのデータをコピーして出せるようなAddGameObjectの関数を作る必要がある。
	// かー、生成したらreturnでptr返ってくるから、そこに対してデータをセットするか。この場合はreturnで帰ってきたポインタを使って敵データをセットする関数を別途作った方が良い。
	Manager::GetCurrentScene()->AddGameObject<FieldEnemy>(0);
}