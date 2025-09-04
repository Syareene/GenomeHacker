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
	// OBB-OBB
	// OBB-AABB
	// OBB-Sphere
};