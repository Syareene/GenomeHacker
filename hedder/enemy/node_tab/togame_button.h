#pragma once

#include "object/ui/button.h"

class ToGameButton : public Button
{
public:
	// dnaタブからゲーム側に行く処理
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ToGame();
private:
	const Vector2 BUTTON_SIZE = Vector2(250.0f, 75.0f);
	const Vector2 BUTTON_POS = Vector2(BUTTON_SIZE.x / 2, BUTTON_SIZE.y / 2);
};