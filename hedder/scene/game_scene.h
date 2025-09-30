#pragma once

#include "base_scene.h"

class GameScene : public Scene
{
public:
	enum class State
	{
		NORMAL, // 通常時
		ESC_MENU, // escメニュー表示時
		DNA_TAB, // DNAタブ表示時
	};
private:
	class Audio* m_BGM;
	State m_State = State::NORMAL; // 初期状態は通常時
	State m_BeforeState = State::NORMAL; // 1f前の状態を保持する変数
	State m_WillState = State::NORMAL; // 次の状態を保持する変数(状態切り替え時に使用)
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	inline void SetState(const State& state) { m_WillState = state; } // 状態を設定する関数
	inline const State GetState() const { return m_State; } // 現在の状態を取得する関数
	inline const State GetBeforeState() const { return m_BeforeState; } // 1f前の状態を取得する関数
	inline const bool IsStateChanged() const { return m_State != m_BeforeState; } // 状態が変化したかどうかを取得する関数
};