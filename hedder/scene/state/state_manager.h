#pragma once

#include "scene/state/base_state.h"
#include <typeindex>


class StateManager
{
public:
	template <typename T>
	inline T* SetState()
	{
		// 次のstateをセット（型情報も保持）
		m_WillState = std::make_unique<T>();
		m_WillStateType = std::type_index(typeid(T));
		return static_cast<T*>(m_WillState.get());
	}
	inline State* GetState(void) const
	{
		return m_State.get();
	}

	// セットされたStateを確認し移行する
	void MoveState()
	{
		m_PreviousStateType = m_StateType;
		if (m_WillState != nullptr)
		{
			if (m_State)
			{
				m_State->Uninit();
			}
			m_State = std::move(m_WillState);
			if (m_State)
			{
				m_State->Init();
				m_StateType = std::type_index(typeid(*m_State));
			}
			else
			{
				m_StateType = std::type_index(typeid(void));
			}
			m_WillStateType = std::type_index(typeid(void));
			m_StateChanged = (m_PreviousStateType != m_StateType);
		}
		else
		{
			m_StateChanged = false;
		}
	}

	bool IsStateChanged() const
	{
		return m_StateChanged;
	}

	// フラグを外部からリセットする
	void ResetStateChanged()
	{
		m_StateChanged = false;
	}

private:
	std::unique_ptr<State> m_State = nullptr;
	std::unique_ptr<State> m_WillState = nullptr;

	std::type_index m_StateType = std::type_index(typeid(void));
	std::type_index m_WillStateType = std::type_index(typeid(void));
	std::type_index m_PreviousStateType = std::type_index(typeid(void));

	bool m_StateChanged = false;
};