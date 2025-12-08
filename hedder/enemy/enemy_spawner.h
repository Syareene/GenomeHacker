#pragma once

#include <list>
#include <memory>
#include "scene/manager.h"
#include "enemy/base_data/enemy_list.h"
#include "object/system_object.h"

#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "collider/sphere.h"

class EnemySpawner : public SystemObject
{
	// 基本はgameSceneにおいて、呼び出したときにオブジェクトを生成するだけのクラス
	// 今後機能が拡張するとウェーブ管理とかもするので敵の召喚をgameScene側から関数を通じて指定するのではなく
	// こっち側で判断するように
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void UpdateFinal() override {};
private:
	//EnemySpawner() = default; // newできないように->この場合は自身でインスタンスの所在を持ってないと行けなくはなるけどね。一旦検討
	template<typename T>
	bool SpawnEnemy(Vector3 spawn_pos = {0.0f, 0.0f, 0.0f})
	{
		// 敵を出す処理

		// 一時的処理として30体以上でないようにする
		if (m_SpawnCount >= 30) return false;

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
		// データが見つからなかった
		//assert(0 && "敵のデータが登録されておらず、インスタンス化できませんでした");
		//return false;
	}
	// 敵の元データを格納する変数
	//std::list<std::unique_ptr<EnemyBase>> m_EnemyBaseList; // 敵の元データを格納するリスト
	int m_SpawnTimer = 0; // 敵を出すタイマー
	int m_SpawnCount = 0; // 出した敵の数(temp)
	//void SetEnemyData(FieldEnemy* set_target, int target_id); // ポインタを受け取って特定の敵の初期化情報をセットする関数
};