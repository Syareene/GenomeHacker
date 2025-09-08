#pragma once

#include "3d_object.h"

class Field : public Object3D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
};