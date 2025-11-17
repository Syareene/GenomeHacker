#pragma once

#include "base_scene.h"

class TitleScene : public Scene
{
public:
	void Init() override;
	//void Uninit() override;
	void Update() override;
	void Draw() override;
};