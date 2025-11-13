#pragma once

#include <list>
#include <memory>
#include "enemy/dna_screen_script.h"
#include "object/ui/button.h"
#include "enemy/base_data/enemy_base.h"

#include "object/system_object.h"


template<typename T>
concept EnemyBaseType = std::is_base_of_v <EnemyBase, T>;

// システムオブジェクトを作ってそれを継承しつつ実体化しないといけない
class EnemyList : public SystemObject
{
public:
	EnemyList() {};
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	std::list<std::unique_ptr<EnemyBase>>& GetEnemyBases(void)
	{
		return m_EnemyBaseList;
	}
	template <EnemyBaseType T>
	T* GetEnemyBase(void)
	{
		for (auto& enemyBase : m_EnemyBaseList)
		{
			if (auto ptr = dynamic_cast<T*>(enemyBase.get()))
			{
				return ptr; // 見つかったらポインタを返す
			}
		}
		return nullptr;
	}
private:
	std::list<std::unique_ptr<EnemyBase>> m_EnemyBaseList; // 敵の元データを保存するリスト
};