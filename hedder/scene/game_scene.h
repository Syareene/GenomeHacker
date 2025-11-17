#pragma once

#include "base_scene.h"
#include "scene/state/state_manager.h"

class GameScene : public Scene
{
public:
	enum class State
	{
		NORMAL,
		DNA_TAB,
		DNA_EDIT,
	};

private:
	class Audio* m_BGM;
	StateManager m_StateManager; // State管理クラス
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// StateManager wrapper
	void SetState(State s);
	State GetState() const;
	bool IsStateChanged() const;
};