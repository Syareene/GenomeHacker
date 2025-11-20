#pragma once

#include "object/system_object.h"

class EnemyDnaList : public SystemObject
{
public:
	// dnaタブからゲーム側に行く処理
	void Init() override;
	void Uninit() override;
	void Update() override;
	void UpdateFinal() override;
private:
	const int COLUMN_NUM = 5; // 横に並べる数
	//const int ROW_NUM = 3; // 縦に並べる数
	const float WIDTH_SIZE = 150.0f; // 横のサイズ
	const float HEIGHT_SIZE = 150.0f; // 縦のサイズ
};