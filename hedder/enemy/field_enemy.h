#pragma once

#include "object/3d_object.h"

class EnemyBase; // 前方宣言

class FieldEnemy : public Object3D
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	
	void SetEnemyBase(EnemyBase* base) { m_EnemyBase = base; }
	EnemyBase* GetEnemyBase() const { return m_EnemyBase; }
	void SetCurrentHP(float hp) { m_CurrentHP = hp; }
	float GetCurrentHP() const { return m_CurrentHP; }
private:
	EnemyBase* m_EnemyBase = nullptr; // 自身の元データへのポインタ
	float m_CurrentHP = 0.0f; // 現在のHP
};