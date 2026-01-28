#pragma once

#include "object/3d_object.h"

class EnemyBullet : public Object3D
{
public:
	static constexpr int MAX_OBJECTS = 500; // 敵の弾の最大数

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
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer = nullptr;
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	int m_LifeTime = 300; // 敵の弾は少し長めに設定
	int m_OwnerEnemyID = -1; // この弾を発射した敵の種類ID
};