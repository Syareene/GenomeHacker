#pragma once

#include <list>
#include <memory>
#include "object/3d_object.h"
#include "object/2d_object.h"

class Scene
{
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template<typename T>
	T* AddGameObject(int layerNum)
	{
		// 中で型を比べる
		if constexpr (!std::is_base_of_v<Object3D, T> && !std::is_base_of_v<Object2D, T>)
		{
			static_assert(false, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		// 初期化処理実行
		std::unique_ptr<T> game_obj = std::make_unique<T>();
		// 初期化に引数ありの場合はreturnから実行する形にかな
		game_obj->Init();

		if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合

			// layerNumとコンテナのサイズを比べる
			if (layerNum < 0)
			{
				// 
			}
			else if (layerNum >= static_cast<int>(m_Objects2D.size()))
			{
				// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
				for (int i = static_cast<int>(m_Objects2D.size()); i <= layerNum; i++)
				{
					// 追加
					m_Objects2D.emplace_back(std::list<std::unique_ptr<Object2D>>());
				}
			}
			// layerNum分iteratorを進める
			auto it = m_Objects2D.begin();
			std::advance(it, layerNum);
			// layerNumの位置に追加
			it->push_back(std::unique_ptr<Object2D>(std::move(game_obj)));
			// スマポで管理しつつも生ポインタで返すように
			return static_cast<T*>(it->back().get());
		}
		else if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合

			// layerNumとコンテナのサイズを比べる
			if (layerNum < 0)
			{
				// 
			}
			else if (layerNum >= static_cast<int>(m_Objects3D.size()))
			{
				// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
				for (int i = static_cast<int>(m_Objects3D.size()); i <= layerNum; i++)
				{
					// 追加
					m_Objects3D.emplace_back(std::list<std::unique_ptr<Object3D>>());
				}
			}
			// layerNum分iteratorを進める
			auto it = m_Objects3D.begin();
			std::advance(it, layerNum);
			// layerNumの位置に追加
			it->push_back(std::unique_ptr<Object3D>(std::move(game_obj)));
			// スマポで管理しつつも生ポインタで返すように
			return static_cast<T*>(it->back().get());
		}
		return nullptr; // ここに来ることはないはず
	}

	template <typename T>
	T* GetGameObject()
	{
		// 中で型を比べる
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr; // 見つかったらポインタを返す
					}
				}
			}
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr; // 見つかったらポインタを返す
					}
				}
			}
		}
		else
		{
			// 型が違う場合はエラーを出す
			static_assert(false, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		return nullptr; // 見つからなかったらnullptrを返す
	}

	template <typename T>
	std::list<T*> GetGameObjects()
	{
		// 中で型を比べる
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			// 3Dオブジェクトの場合
			std::list<T*> result;
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr); // 見つかったらリストに追加
					}
				}
			}
			return result; // 3Dオブジェクトのリストを返す
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			// 2Dオブジェクトの場合
			std::list<T*> result;
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr); // 見つかったらリストに追加
					}
				}
			}
			return result; // 2Dオブジェクトのリストを返す
		}
		else
		{
			// 型が違う場合はエラーを出す
			static_assert(false, "Unsupported GameObject type");
			return std::list<T*>(); // 型が違う場合は空のリストを返す
		}
		return std::list<T*>(); // 見つからなかったら空のリストを返す
	}
private:
	std::list<std::list<std::unique_ptr<Object3D>>> m_Objects3D;
	std::list<std::list<std::unique_ptr<Object2D>>> m_Objects2D;
};