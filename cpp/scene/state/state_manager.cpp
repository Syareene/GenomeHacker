#include "main.h"
#include "scene/state/state_manager.h"
#include "scene/state/base_state.h"

StateManager::~StateManager() = default;

void StateManager::MoveState()
{
	// state変更が予定されている場合、stateを移動させる
	if (m_WillState)
	{
		if (m_State)
		{
			m_State->Uninit();
		}
		m_State = std::move(m_WillState);
		if (m_State)
		{
			m_State->Init();
		}
		m_StateChanged = true;
	}
	else
	{
		m_StateChanged = false;
	}
}