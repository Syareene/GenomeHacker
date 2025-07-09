#pragma once

#include "2d_object.h"

class Polygon2D : public Object2D
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
};