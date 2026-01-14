#pragma once

#include "enemy/node_tab/tab_base.h"

class DeathTab : public TabBase
{
public:
	void Init(const unsigned int& playerId, Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};