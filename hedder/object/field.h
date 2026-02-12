#pragma once

#include "3d_object.h"

class Field : public Object3D
{
public:
	static constexpr bool ENABLE_INSTANCING = false;
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
};