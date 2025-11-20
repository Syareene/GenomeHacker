#pragma once

#include <memory>
#include <utility>

#include "scene/state/base_state.h"

class StateManager
{
public:
	StateManager() = default;
	~StateManager();

	template <typename T>
	// state変更関数。変更したstateに対して値をセットしたい場合はこの関数の戻り値を使うこと
	inline T* SetState()
	{
		m_WillState = std::make_unique<T>();
		return static_cast<T*>(m_WillState.get());
	}

	inline State* GetState() const
	{
		return m_State.get();
	}

	void MoveState();

	template <typename T>
	bool IsCurrentState() const
	{
		return dynamic_cast<T*>(m_State.get()) != nullptr;
	}

	bool IsStateChanged() const
	{
		return m_StateChanged;
	}

	void ResetStateChanged()
	{
		m_StateChanged = false;
	}

private:
	std::unique_ptr<State> m_State;
	std::unique_ptr<State> m_WillState;
	bool m_StateChanged = false;
};