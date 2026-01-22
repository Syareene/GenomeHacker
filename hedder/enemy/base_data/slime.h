#pragma once

#include "enemy/base_data/enemy_base.h"


class Slime : public EnemyBase
{
public:
	EnemyBase* Register(const unsigned int& playerId) override; // 登録処理
};