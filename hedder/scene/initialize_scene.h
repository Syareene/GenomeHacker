#pragma once

#include "base_scene.h"

class ImageDraw;

class InitializeScene : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	ImageDraw* m_LogoImage = nullptr;
	int m_Timer = 0;
};