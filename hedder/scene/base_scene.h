#pragma once

#include <list>
#include <memory>
#include "object/gameObject.h"

class Scene
{
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template <typename T>
	T* AddGameObject(int layerNum)
	{
		std::unique_ptr<T> GameObj = std::make_unique<T>();
		// 初期化に引数ありの場合はreturnから実行する形にかな
		GameObj->Init();

		
		// layerNumとコンテナのサイズを比べる
		if (layerNum < 0)
		{
			//throw std::out_of_range("レイヤーに負の数を指定することはできません");
		}
		else if (layerNum >= static_cast<int>(m_GameObjects.size()))
		{
			// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
			for (int i = static_cast<int>(m_GameObjects.size()); i <= layerNum; i++)
			{
				// 追加
				m_GameObjects.emplace_back(std::list<std::unique_ptr<GameObject>>());
			}
		}
		

		// layerNum分iteratorを進める
		auto it = m_GameObjects.begin();
		std::advance(it, layerNum);
		// layerNumの位置に追加
		it->push_back(std::unique_ptr<GameObject>(std::move(GameObj)));
		// スマポで管理しつつも生ポインタで返すように
		return static_cast<T*>(it->back().get());
	}

	template <typename T>
	T* GetGameObject()
	{
		for (auto& gameObjects : m_GameObjects)
		{
			for (auto& gameObject : gameObjects)
			{
				if (auto ptr = dynamic_cast<T*>(gameObject.get()))
				{
					return ptr;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::list<T*> GetGameObjects()
	{
		std::list<T*> objects;
		for (auto& gameObjects : m_GameObjects)
		{
			for (auto& gameObject : gameObjects)
			{
				if (auto ptr = dynamic_cast<T*>(gameObject.get()))
				{
					objects.emplace_back(ptr);
				}
			}
		}
		return objects;
	}
private:
	std::list<std::list<std::unique_ptr<GameObject>>> m_GameObjects;
};