#pragma once

#include "object/system_object.h"

class State;

class EnemyDnaList : public SystemObject
{
public:

	// dnaタブからゲーム側に行く処理
	void Init(State* ptr);
	void Uninit() override;
	void Update() override;
	void UpdateFinal() override;
private:
	static constexpr float WIDTH_SIZE = 150.0f; // 横のサイズ
	static constexpr float HEIGHT_SIZE = 150.0f; // 縦のサイズ
	static constexpr Vector2 START_POS = Vector2(200.0f, 300.0f); // 開始位置
	static constexpr Vector2 BUTTON_PADDING = Vector2(WIDTH_SIZE / 8.0f, HEIGHT_SIZE / 12.0f); // ボタン同士の余白

	static constexpr int COLUMN_NUM = 5; // 横に並べる数
	//const int ROW_NUM = 3; // 縦に並べる数
};