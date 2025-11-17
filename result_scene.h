#pragma once

#include "scene/base_scene.h"

// 課題用にリザルトシーン置いとく
class ResultScene : public Scene
{
public:
	void Init() override;
	//void Uninit() override;
	void Update() override;
	void Draw() override;
};