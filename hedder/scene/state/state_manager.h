#pragma once

#include <memory>
#include <utility>

#include "scene/state/base_state.h" // Ensure State is a complete type for unique_ptr

class StateManager
{
public:
	StateManager() = default;
	~StateManager(); // defined in cpp

	template <typename T>
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