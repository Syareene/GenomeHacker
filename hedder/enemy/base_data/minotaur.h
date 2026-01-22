#pragma once

#include "enemy/base_data/enemy_base.h"


class Minotaur : public EnemyBase
{
public:
	EnemyBase* Register(const unsigned int& playerId) override; // 登録処理
};