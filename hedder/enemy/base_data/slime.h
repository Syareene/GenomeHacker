#pragma once

#include "enemy/base_data/enemy_base.h"


class Slime : public EnemyBase
{
public:
	void Register() override; // 登録処理
};