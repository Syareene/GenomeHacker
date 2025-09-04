#pragma once

#include <list>
#include <memory>
#include "manager.h"

#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"

class EnemySpawner
{
	// 基本はgameSceneにおいて、呼び出したときにオブジェクトを生成するだけのクラス
	// 今後機能が拡張するとウェーブ管理とかもするので敵の召喚をgameScene側から関数を通じて指定するのではなく
	// こっち側で判断するように
public:
	void Init();
	void Uninit();
	void Update();
private:
	//EnemySpawner() = default; // newできないように->この場合は自身でインスタンスの所在を持ってないと行けなくはなるけどね。一旦検討
	template<typename T>
	void SpawnEnemy(Vector3 spawn_pos = {0.0f, 0.0f, 0.0f})
	{
		// 敵を出す処理

		// objectをフィールドに追加
		FieldEnemy* enemy = Manager::GetCurrentScene()->AddGameObject<FieldEnemy>(0);


		// 受け取った引数のポインタの型を確認し、変数に保存されている敵の元データを参照しセットする
		for (const auto& base : m_EnemyBaseList)
		{
			if (dynamic_cast<T*>(base.get()))
			{
				// 一致したのでデータをセットする

				// fieldenemyに欲しいデータ上げておく
				// base_dataへのリファレンス(不動及びnodeデータはここから引っ張ってくる)
				// 後は変動するステータス: 現在HP

				enemy->SetEnemyBase(base.get());
				enemy->SetCurrentHP(base.get()->GetMaxHealth());

				// スポーン時の座標及びテクスチャによるズレを補正
				enemy->SetPosition(spawn_pos + base.get()->GetDrawPosDiff());
				enemy->SetScale(enemy->GetScale().mul(base.get()->GetDrawScaleDiff()));

				// データセットしたのでループから抜ける
				break;
			}
		}
	}
	// 敵の元データを格納する変数
	std::list<std::unique_ptr<EnemyBase>> m_EnemyBaseList; // 敵の元データを格納するリスト
	int m_SpawnTimer = 0; // 敵を出すタイマー
	int m_SpawnCount = 0; // 出した敵の数(temp)
	//void SetEnemyData(FieldEnemy* set_target, int target_id); // ポインタを受け取って特定の敵の初期化情報をセットする関数
};