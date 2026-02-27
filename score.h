#pragma once

#include "object/2d_object.h"

class Score : public Object2D
{
public:
	static constexpr bool ENABLE_INSTANCING = false;
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void AddScore(int score) { m_Score += score; } // スコア加算
	void SetScore(int score) { m_Score = score; } // スコア設定
private:
	int m_Score = 0;
	constexpr static Vector3 SHOW_POSITION = Vector3(50.0f, 50.0f, 0.0f); // スコア表示位置
	constexpr static Vector3 SHOW_SCALE = Vector3(75.0f, 75.0f, 1.0f); // スコア表示スケール
	constexpr static int SHOWABLE_DIGITS = 5; // 表示可能な桁数

};