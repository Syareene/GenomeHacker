#pragma once

#include "object/3d_object.h"

class Explosion : public Object3D
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	int m_FrameCount = 0; // フレームカウント
};