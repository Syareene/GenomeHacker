#pragma once
#include "collider/collision.h"

class AABBCube : public Collision
{
public:
	void SetAABBProperty(const Vector3& center, const Vector3& size) override
	{
		// 衝突判定のプロパティ設定
		Collision::SetAABBProperty(center, size);
	}

	void DrawCollider() const override
	{
		
	}

	// AABB-AABB
	// AABB-OBB
	// AABB-Sphere
};

class OBBCube : public Collision
{
public:
	void SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation) override
	{
		// 衝突判定のプロパティ設定
		Collision::SetOBBProperty(center, size, rotation);
	}

	void DrawCollider() const override
	{
		
	}

	// OBB-OBB
	// OBB-AABB
	// OBB-Sphere
};