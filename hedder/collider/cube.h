#pragma once
#include "collider/collision.h"

class AABBCube : public Collision
{
public:
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

	void DrawCollider() const override
	{
		
	}

	// OBB-OBB
	// OBB-AABB
	// OBB-Sphere
};