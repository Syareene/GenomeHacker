#pragma once

#include "enemy/node_tab/tab_base.h"

class AttackTab : public TabBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};