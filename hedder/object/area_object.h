#pragma once

#include "3d_object.h"

class AreaObject : public Object3D
{
public:
	static void SetPipelineState();
	void UpdateGPUData(InstanceBufferData& data) override;
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;

	inline void SetMaxDuration(const int& duration) { m_MaxDuration = duration; }
	inline const int GetMaxDuration() const { return m_MaxDuration; }
	inline void SetAreaDamage(const float& damage) { m_AreaDamage = damage; }
	inline const float GetAreaDamage() const { return m_AreaDamage; }
	inline void ResetDuration() { m_Duration = 0; }
	inline const int GetCurrentDuration() const { return m_Duration; }
	inline void SetOwnerEnemyID(const int enemyID) { m_OwnerEnemyID = enemyID; }
	inline const int GetOwnerEnemyID() const { return m_OwnerEnemyID; }
private:
	static constexpr int APPLY_COLLIDER = 45; // 判定開始始まるフレーム
	float m_AreaDamage = 1.0f; // エリアのダメージ(仮)
	int m_MaxDuration = 0; // エリアの最大持続時間(フレーム数)
	int m_Duration = 0;
	int m_OwnerEnemyID = -1; // この弾を発射した敵の種類ID
};