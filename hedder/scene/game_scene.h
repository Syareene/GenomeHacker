#pragma once

#include "base_scene.h"

class GameScene : public Scene
{
private:
	class Audio* m_BGM;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};