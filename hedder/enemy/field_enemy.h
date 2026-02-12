#pragma once

#include "object/3d_object.h"

class EnemyBase; // 前方宣言

class FieldEnemy : public Object3D
{
public:
	void Init(EnemyBase* base, Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static void SetPipelineState();
	void UpdateGPUData(InstanceBufferData& data) override;

	inline void SetEnemyBase(EnemyBase* base) { m_EnemyBase = base; }
	inline EnemyBase* GetEnemyBase() const { return m_EnemyBase; }
	inline unsigned int GetLiveTime() const { return m_LiveTime; }
	inline void SetCurrentHP(const float hp) { m_CurrentHP = hp; }
	inline const float GetCurrentHP() const { return m_CurrentHP; }
	inline void SetAttackNodeTime(const int time) { m_AttackNodeTime = time; }
	inline const int GetAttackNodeTime() const { return m_AttackNodeTime; }
	inline void SetMoveNodeTime(const int time) { m_MoveNodeTime = time; }
	inline const int GetMoveNodeTime() const { return m_MoveNodeTime; }
	inline void SetDeathNodeTime(const int time) { m_DeathNodeTime = time; }
	inline const int GetDeathNodeTime() const { return m_DeathNodeTime; }
	inline void SetAttackNodeCDSum(const int cd) { m_AttackNodeCDSum = cd; }
	inline const int GetAttackNodeCDSum() const { return m_AttackNodeCDSum; }
	inline void SetMoveNodeCDSum(const int cd) { m_MoveNodeCDSum = cd; }
	inline const int GetMoveNodeCDSum() const { return m_MoveNodeCDSum; }
	inline void SetDeathNodeCDSum(const int cd) { m_DeathNodeCDSum = cd; }
	inline const int GetDeathNodeCDSum() const { return m_DeathNodeCDSum; }
	inline Vector3 GetPreviousPosition() const { return m_PreviousPosition; }
	inline void UpdatePreviousPosition() { m_PreviousPosition = GetPosition(); }	
private:
	constexpr static Vector3 COLLIDER_SCALE = Vector3(0.5f, 0.5f, 0.5f);
	constexpr static Vector2 DELETE_POS = Vector2(30.0f, 30.0f);
	bool m_IsHit = false; // 当たったかどうかのフラグ
	EnemyBase* m_EnemyBase = nullptr; // 自身の元データへのポインタ
	unsigned int m_LiveTime = 0; // 生存時間(フレーム)
	int m_AttackNodeTime = 0;
	int m_AttackNodeCDSum = 0;
	int m_MoveNodeTime = 0;
	int m_MoveNodeCDSum = 0;
	int m_DeathNodeTime = 0;
	int m_DeathNodeCDSum = 0;
	float m_CurrentHP = 0.0f; // 現在のHP
	Vector3 m_PreviousPosition = Vector3(0.0f, 0.0f, 0.0f); // 前フレームの位置
};