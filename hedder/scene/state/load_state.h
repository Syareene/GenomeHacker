#pragma once

#include <list>
#include <memory>
#include <string>
#include "scene/state/base_state.h"


class LoadState : public State
{
public:
	// ロード用State

	// 先に読み込みしときたいものはstaticでそれぞれのオブジェクトに持たせとく(or std::function)
	// とりあえずここはサンプルとしてloadは作成しておく
	static void Load();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	bool m_IsLoadComplete = false; // ロード完了フラグ
};