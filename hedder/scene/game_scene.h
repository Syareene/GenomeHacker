#pragma once

#include "base_scene.h"

class GameScene : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	//void Draw() override;
private:
	class Audio* m_BGM;
};