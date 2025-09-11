#pragma once

#include "object/3d_object.h"

class EnemyBase; // 前方宣言

class FieldEnemy : public Object3D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	
	inline void SetEnemyBase(EnemyBase* base) { m_EnemyBase = base; }
	inline EnemyBase* GetEnemyBase() const { return m_EnemyBase; }
	inline void SetCurrentHP(const float hp) { m_CurrentHP = hp; }
	inline const float GetCurrentHP() const { return m_CurrentHP; }
private:
	bool m_IsHit = false; // 当たったかどうかのフラグ
	EnemyBase* m_EnemyBase = nullptr; // 自身の元データへのポインタ
	float m_CurrentHP = 0.0f; // 現在のHP
};