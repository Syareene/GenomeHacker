#pragma once

#include "scene/state/base_state.h"


class SettingState : public State
{
public:
	// ゲーム基本用State
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
