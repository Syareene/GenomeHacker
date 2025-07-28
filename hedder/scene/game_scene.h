#pragma once

#include "base_scene.h"

class GameScene : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	//void Draw() override;
private:
	class Audio* m_BGM;
	enum class State
	{
		NORMAL, // 通常時
		ESC_MENU, // escメニュー表示時
		DNA_TAB, // DNAタブ表示時
	};

   State m_state = State::NORMAL; // 初期状態は通常時
};