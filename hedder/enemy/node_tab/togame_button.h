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
};