#pragma once

#include "object/ui/button.h"

class DNAButton : public Button
{
public:
	// ゲーム側からdnaタブに行くときの処理
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ToDNATab();
private:
	static constexpr Vector2 BUTTON_SIZE = Vector2(200.0f, 100.0f);
	static constexpr Vector2 BUTTON_POS = Vector2(200.0f * 0.5f, SCREEN_HEIGHT - (100.0f * 0.5f));
};