#pragma once

#include <list>
#include <vector>
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
		T* GameObj = new T();
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
				m_GameObjects.emplace_back(std::list<GameObject*>());
			}
		}

		// layerNum分iteratorを進める
		auto it = m_GameObjects.begin();
		std::advance(it, layerNum);
		// layerNumの位置に追加
		it->emplace_back(GameObj);

		return GameObj;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (auto& gameObjects : m_GameObjects)
		{
			for (auto& gameObject : gameObjects)
			{
				if (dynamic_cast<T*>(gameObject))
				{
					return dynamic_cast<T*>(gameObject);
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (auto& gameObjects : m_GameObjects)
		{
			for (auto& gameObject : gameObjects)
			{
				if (dynamic_cast<T*>(gameObject))
				{
					objects.emplace_back(dynamic_cast<T*>(gameObject));
				}
			}
		}
		return objects;
	}
private:
	std::list<std::list<GameObject*> > m_GameObjects;
};