#pragma once

#include "enemy/node_tab/tab_base.h"

class MoveTab : public TabBase
{
public:
	//void Init(const unsigned int& playerId, Transform trans = Transform()) override;
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};