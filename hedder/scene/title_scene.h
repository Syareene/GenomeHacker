#pragma once

#include "base_scene.h"

class TitleScene : public Scene
{
public:
	void Init() override;
	//void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	constexpr static Vector3 CAMERA_START_POS = Vector3(0.0f, 3.0f, -5.0f);
	constexpr static Vector3 TITLE_IMAGE_POS = Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 8, 0.0f);
	constexpr static Vector3 TITLE_IMAGE_SCALE = Vector3(1024.0f, 576.0f, 1.0f);
	constexpr static Vector2 BUTTON_POS = Vector2(SCREEN_WIDTH / 2, 600.0f);
	constexpr static Vector2 BUTTON_SCALE = Vector2(500.0f, 140.0f);
};