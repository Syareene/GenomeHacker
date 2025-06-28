#pragma once

#include "object/gameObject.h"

class Explosion : public GameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	int m_FrameCount = 0; // フレームカウント
};