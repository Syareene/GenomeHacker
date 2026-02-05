#pragma once

#include "enemy/node_tab/tab_base.h"

class DeathTab : public TabBase
{
public:
	void Init(const unsigned int& playerId, Transform trans = Transform()) override;
	void ModifyTimeLine() override; // タイムラインを修正する
};