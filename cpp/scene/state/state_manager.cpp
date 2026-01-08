#include "main.h"
#include "scene/state/state_manager.h"
#include "scene/state/base_state.h"
#include <algorithm>

StateManager::~StateManager() = default;

void StateManager::MoveState()
{
	// state変更が予約されている場合、stateを移動させる
	if (m_WillState.has_value())
	{
		auto& it = m_WillState.value();

		// 今のstateと次のstateが同じならスキップ
		if (!m_StateStack.empty() && m_StateStack.begin() == it)
		{
			m_StateChanged = false;
			m_WillState.reset(); // 予約をクリア
			return;
		}

		// 現在のステート（先頭）でUninitを呼び出す
		if (!m_StateStack.empty())
		{
			m_StateStack.front()->Uninit();
		}

		// 予約されたイテレータが指す要素を先頭に持ってくる
		std::rotate(m_StateStack.begin(), it, it + 1);

		// 新しいステートでInitを呼び出す
		m_StateStack.front()->Init();
		m_StateChanged = true;

		m_WillState.reset(); // state変更予約をクリア
	}
	else
	{
		m_StateChanged = false;
	}
}