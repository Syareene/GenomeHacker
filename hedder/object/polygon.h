#pragma once

#include "2d_object.h"

class Polygon2D : public Object2D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
};