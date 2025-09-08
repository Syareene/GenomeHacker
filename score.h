#pragma once

#include "object/2d_object.h"

class Score : public Object2D
{
public:
	void Init(Transform trans = Transform()) override; // 
	void Uninit() override; // 
	void Update() override; // 
	void Draw() override; // 
	void AddScore(int score) { m_Score += score; } // スコア加算
	void SetScore(int score) { m_Score = score; } // スコア設定
private:
	int m_Score = 0;
	const static int ShowableDigits = 5; // 表示可能な桁数

};