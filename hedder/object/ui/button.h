#pragma once

#include "ui.h"

class Button : public UI
{
private:

public:
	void Init() override; // ボタン登録処理
	void Uninit() override; // ボタン終了処理
	void Update() override; // ボタン更新処理
	void Draw() override; // ボタン描画処理
}