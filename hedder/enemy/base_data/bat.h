#pragma once

#include "enemy/base_data/enemy_base.h"


class Bat : public EnemyBase
{
public:
	EnemyBase* Register(const unsigned int& playerId) override; // 登録処理
private:
	static constexpr float SCORE = 10.0f; // スコア
	static constexpr float MOVE_X_SPEED = 0.1f; // X方向移動速度
	static constexpr float HEALTH = 1.0f; // 体力
	static constexpr Vector3 DRAW_POS_DIFF = { 0.0f, 0.0f, 0.0f }; // 描画時の位置の差分
	static constexpr Vector3 DRAW_SCALE_DIFF = { 1.5f, 1.5f, 1.5f }; // 描画時のスケールの差分
};