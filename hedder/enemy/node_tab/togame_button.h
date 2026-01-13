#pragma once

#include "object/ui/button.h"

class ToGameButton : public Button
{
public:
	// dnaタブからゲーム側に行く処理
	ToGameButton() = default; // デフォルトコンストラクタ
	virtual ~ToGameButton() {}
	ToGameButton(ToGameButton&&) noexcept = default; // ムーブコンストラクタ
	ToGameButton& operator=(ToGameButton&&) noexcept = default; // ムーブ代入演算子
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void ToGame();
private:
	static constexpr Vector2 BUTTON_SIZE = Vector2(250.0f, 75.0f);
	static constexpr Vector2 BUTTON_POS = Vector2(BUTTON_SIZE.x / 2, BUTTON_SIZE.y / 2);
};