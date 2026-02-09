#pragma once

#include "enemy/base_data/enemy_base.h"


class Slime : public EnemyBase
{
public:
	EnemyBase* Register(const unsigned int& playerId) override; // 登録処理
private:
	static constexpr float SCORE = 1.0f; // スコア
	static constexpr float HEALTH = 1.0f; // 体力
	static constexpr Vector3 DRAW_POS_DIFF = { 0.0f, 0.0f, 0.0f }; // 描画時の位置の差分
	static constexpr Vector3 DRAW_SCALE_DIFF = { 2.0f, 2.0f, 2.0f }; // 描画時のスケールの差分
};