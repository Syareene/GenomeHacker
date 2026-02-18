#pragma once

#include <list>
#include <memory>
#include "scene/manager.h"
#include "enemy/base_data/enemy_list.h"
#include "object/system_object.h"
#include "scene/base_scene.h"

#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "collider/sphere.h"

// 敵を一定間隔で召喚するクラス
class EnemySpawner : public SystemObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void UpdateFinal() override {};
	void EnemyKilled()
	{
		m_SpawnedCount--;
	}
private:
	//EnemySpawner() = default; // newできないように->この場合は自身でインスタンスの所在を持ってないと行けなくはなるけどね。一旦検討
	template<typename T>
	bool SpawnEnemy(Vector3 spawn_pos = {0.0f, 0.0f, 0.0f})
	{
		// 敵を出す処理

		// 一定以上でないようにする
		if (m_SpawnedCount >= ENEMY_MAX_AMOUNT) return false;

		// objectをフィールドに追加
		Transform trans;
		trans.SetPosition(spawn_pos);
		trans.SetRotation({ 0.0f, 0.0f, 0.0f });
		trans.SetScale({ 1.0f, 1.0f, 1.0f });

		FieldEnemy* enemy = Manager::GetCurrentScene()->AddGameObject<FieldEnemy>(0, trans);


		// 受け取った引数のポインタの型を確認し、変数に保存されている敵の元データを参照しセットする
		EnemyBase* base_data = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBase<T>();

		// fieldenemyに欲しいデータ上げておく
		// base_dataへのリファレンス(不動及びnodeデータはここから引っ張ってくる)
		// 後は変動するステータス: 現在HP

		enemy->SetEnemyBase(base_data);
		enemy->SetCurrentHP(base_data->GetMaxHealth());

		// スポーン時の座標及びテクスチャによるズレを補正->一旦上に移行したので書くならそっちに書く形
		enemy->SetScale(enemy->GetScale().mul(base_data->GetDrawScaleDiff()));

		// コライダをセット

		// データセットしたのでループから抜ける
		return true;
	}

	bool SpawnEnemyByData(EnemyBase* target_data, Vector3 spawn_pos = { 0.0f, 0.0f, 0.0f })
	{
		// 一定以上でないようにする
		if (m_SpawnedCount >= ENEMY_MAX_AMOUNT) return false;

		// objectをフィールドに追加
		Transform trans;
		trans.SetPosition(spawn_pos);
		trans.SetRotation({ 0.0f, 0.0f, 0.0f });
		trans.SetScale({ 1.0f, 1.0f, 1.0f });

		FieldEnemy* enemy = Manager::GetCurrentScene()->AddGameObject<FieldEnemy>(0, target_data, trans);

		// fieldenemyに欲しいデータ上げておく
		// base_dataへのリファレンス(不動及びnodeデータはここから引っ張ってくる)
		// 後は変動するステータス: 現在HP

		enemy->SetCurrentHP(target_data->GetMaxHealth());

		// スポーン時の座標及びテクスチャによるズレを補正->一旦上に移行したので書くならそっちに書く形
		enemy->SetScale(enemy->GetScale().mul(target_data->GetDrawScaleDiff()));

		// コライダをセット

		// 生成したのでtrueを返す
		return true;
	}
	EnemyBase* m_EnemyBaseData = nullptr; // 敵の元データを格納するポインタ(敵を出現させるのに用いる)
	int m_SpawnTimer = 0; // 敵を出すタイマー
	int m_SpawnCount = 0; // 出す敵の数
	constexpr static int DEFAULT_SPAWN_COUNT = 6;
	int m_SpawnedCount = 0; // 出した敵の数(temp)
	int m_EnemyCost = 20;
	int m_TimeCost = 400; // 敵の出現事の間隔とwave間の間隔を決めるための変数
	int m_EnemySpawnInterval = 0; // 敵が出現するまでの間隔
	int m_TimeNextWave = 0; // ウェーブ間の時間
	constexpr static int ENEMY_MAX_AMOUNT = 40;
	constexpr static Vector2 SPAWN_POSITION_RANGE = { -5.0f, 5.0f };
};