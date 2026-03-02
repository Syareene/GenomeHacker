#pragma once

#include "3d_object.h"

class BillBoard : public Object3D
{
public:
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	ID3D11ShaderResourceView* m_Texture = nullptr;
	int m_FrameCount = 0; // フレームカウント
};