#pragma once
#include "collider_base.h"

class AABBCube : public ColliderBase
{
	// AABB-AABB
	// AABB-OBB
	// AABB-Sphere
};

class OBBCube : public ColliderBase
{
	// OBB-OBB
	// OBB-AABB
	// OBB-Sphere
};