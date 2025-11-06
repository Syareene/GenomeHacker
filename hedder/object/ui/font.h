#pragma once

#include "object/2d_object.h"
#include "lib/write_font.h"

class DirectWriteSample : public Object2D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	FontData m_FontData;
};