#pragma once

#include <list>
#include <memory>
#include <string>
#include "scene/state/base_state.h"


class GameBaseState : public State
{
public:
	// ゲーム基本用State
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};