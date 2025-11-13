#pragma once

#include "scene/state/base_state.h"


class StateManager
{
public:
	template <typename T>
	inline void SetState()
	{
		// 次のstateをセット
		m_WillState = std::make_unique<T>();
	}
	inline State* GetState(void)
	{
		return m_State.get();
	}

	// 次のstate移行関数

	// セットされたStateを確認し移行する
	void MoveState()
	{
		if (m_WillState != nullptr)
		{
			m_State->Uninit();
			m_State = std::move(m_WillState);
			m_State->Init();
			m_WillState = nullptr;
		}
	}
	
private:
	std::unique_ptr<State> m_State = nullptr;
	//State m_PreviousState;
	std::unique_ptr<State> m_WillState = nullptr;
};