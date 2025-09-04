#pragma once
#include "collider/collision.h"

class Sphere : public Collision
{
public:
	void Test() override;
	void SetSphereProperty(const Vector3& center, float radius) override
	{
		// 衝突判定のプロパティ設定
		Collision::SetSphereProperty(center, radius);
	}
	// 設定したいプロパティとかは基底クラスにあるから一旦形だけ?

	// Sphere-Sphere
	// Sphere-AABB
	// Sphere-OBB
};