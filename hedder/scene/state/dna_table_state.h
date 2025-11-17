#pragma once

#include <list>
#include <memory>
#include <string>
#include "scene/state/base_state.h"


class DnaTableState : public State
{
public:
	// 敵一覧表示用State
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};