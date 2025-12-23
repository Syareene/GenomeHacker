#pragma once

#include <memory>
#include <utility>
#include <deque>
#include <optional>

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
		// 既にStateが存在している場合はデキューから取得しreturn
		for (auto it = m_StateStack.begin(); it != m_StateStack.end(); ++it)
		{
			if (dynamic_cast<T*>(it->get()))
			{
				// 見つかった場合、そのイテレータを予約してポインタを返す
				m_WillState = it;
				return static_cast<T*>(it->get());
			}
		}
		// ない場合は新規作成してデキューに追加
		m_StateStack.emplace_back(std::make_unique<T>());
		// 追加した要素（末尾）のイテレータを予約
		m_WillState = std::prev(m_StateStack.end());
		return static_cast<T*>(m_StateStack.back().get());
	}

	inline State* GetState() const
	{
		if (m_StateStack.empty()) return nullptr;
		return m_StateStack.front().get();
	}

	void MoveState();

	template <typename T>
	bool IsCurrentState() const
	{
		if (m_StateStack.empty()) return false;
		return dynamic_cast<T*>(m_StateStack.front().get()) != nullptr;
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
	using StateIterator = std::deque<std::unique_ptr<State>>::iterator;
	std::deque<std::unique_ptr<State>> m_StateStack; // キャッシュするためのデキュー(現在のStateは先頭である)
	//std::unique_ptr<State> m_State;
	std::optional<StateIterator> m_WillState; // 次のStateへのイテレータを予約(std::optionalでラップ)
	bool m_StateChanged = false;
};