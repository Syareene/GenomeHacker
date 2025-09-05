#pragma once
#include "lib/vector3.h"

class Collision
{
	// Gameobjectにアタッチする用のプロパティ
public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void DrawCollider(); // デバッグ用の衝突判定の可視化

	// データ取得
	Vector3 GetCenter() const { return m_Center; }
	Vector3 GetSize() const { return m_Size; } // AABBとOBB用
	Vector3 GetRotation() const { return m_Rotation; } // OBB用
	float GetRadius() const { return m_Radius; } // Sphere用
	// データセット
	void SetCenter(const Vector3& center) { m_Center = center; }
	void SetSize(const Vector3& size) { m_Size = size; } // AABB/OBB
	void SetRotation(const Vector3& rotation) { m_Rotation = rotation; } // OBB
	void SetRadius(float radius) { m_Radius = radius; } // Sphere
protected:
	virtual void SetSphereProperty(const Vector3& center, float radius);
	virtual void SetAABBProperty(const Vector3& center, const Vector3& size);
	virtual void SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation);
private:
	// 衝突判定のプロパティ
	Vector3 m_Center = { 0.0f, 0.0f, 0.0f }; // 中心位置
	Vector3 m_Size = { 1.0f, 1.0f, 1.0f }; // サイズ(AABBとOBB用)
	Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f }; // 回転値(OBB用)
	float m_Radius = 1.0f; // Sphereの半径

	// collisiontypeを判定するためにこれをoverrideして使ったほうがいいかなぁって気がしなくもない。
};