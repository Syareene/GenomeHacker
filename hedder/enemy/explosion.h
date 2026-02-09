#pragma once

#include "object/3d_object.h"

class Explosion : public Object3D
{
public:
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	constexpr static int FRAME_MAX = 16; // 最大フレーム数
	int m_FrameCount = 0; // フレームカウント
};