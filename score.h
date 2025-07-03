#pragma once

#include "textureManager.h"
#include "object/gameObject.h"

class Score : public GameObject
{
public:
	void Init() override; // 
	void Uninit() override; // 
	void Update() override; // 
	void Draw() override; // 
	void AddScore(int score) { m_Score += score; } // スコア加算
	void SetScore(int score) { m_Score = score; } // スコア設定
private:
	int m_Score = 0;
	const static int ShowableDigits = 5; // 表示可能な桁数

};