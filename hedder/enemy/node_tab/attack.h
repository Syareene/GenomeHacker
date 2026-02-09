#pragma once

#include "enemy/node_tab/tab_base.h"

class AttackTab : public TabBase
{
public:
	void Init(const unsigned int& playerId, Transform trans = Transform()) override;
};