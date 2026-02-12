#pragma once

#include "object/3d_object.h"

class EnemyBullet : public Object3D
{
public:
	static constexpr size_t MAX_OBJECTS = 1000; // オブジェクトvector最大数。継承先クラスで変更可能。
	static constexpr bool ENABLE_INSTANCING = false;
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	const Vector3 GetVelocity() const { return m_Velocity; }

	// 発射した敵の種類IDを設定する関数
	void SetOwnerEnemyID(const int enemyID) { m_OwnerEnemyID = enemyID; }
	const int GetOwnerEnemyID() const { return m_OwnerEnemyID; }

private:
	constexpr static Vector3 POSITION_OFFSET = Vector3(0.0f, -1.0f, 0.0f);
	constexpr static Vector3 SCALE = Vector3(0.225f, 0.225f, 0.225f);
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer = nullptr;
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	int m_LifeTime = 180; // 弾の生存時間
	int m_OwnerEnemyID = -1; // この弾を発射した敵の種類ID
};