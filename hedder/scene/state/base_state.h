#pragma once

#include <list>
#include <memory>
#include <string>
#include <type_traits>
#include "object/game_object.h"
#include "object/system_object.h"
#include "object/3d_object.h"
#include "object/2d_object.h"

class State
{
public:
	State() = default;
	State(State&& other) noexcept = default;
	virtual ~State();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	void UpdateFinal();

	template<typename T>
	T* AddGameObject(int layerNum, Transform trans = Transform())
	{
		if constexpr (!std::is_base_of_v<Object3D, T> && !std::is_base_of_v<Object2D, T>)
		{
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		// 初期化処理実行
		std::unique_ptr<T> game_obj = std::make_unique<T>();
		game_obj->Init(trans);

		if constexpr (std::is_base_of_v<Object2D, T>)
		{
			if (layerNum < 0)
			{
				//
			}
			else if (layerNum >= static_cast<int>(m_Objects2D.size()))
			{
				for (int i = static_cast<int>(m_Objects2D.size()); i <= layerNum; i++)
				{
					m_Objects2D.emplace_back(std::list<std::unique_ptr<Object2D>>());
				}
			}
			auto it = m_Objects2D.begin();
			std::advance(it, layerNum);
			it->push_back(std::unique_ptr<Object2D>(std::move(game_obj)));
			return static_cast<T*>(it->back().get());
		}
		else if constexpr (std::is_base_of_v<Object3D, T>)
		{
			if (layerNum < 0)
			{
				//
			}
			else if (layerNum >= static_cast<int>(m_Objects3D.size()))
			{
				for (int i = static_cast<int>(m_Objects3D.size()); i <= layerNum; i++)
				{
					m_Objects3D.emplace_back(std::list<std::unique_ptr<Object3D>>());
				}
			}
			auto it = m_Objects3D.begin();
			std::advance(it, layerNum);
			it->push_back((std::move(game_obj)));
			return static_cast<T*>(it->back().get());
		}
		return nullptr; // ここに来ることはないはず
	}

	template<typename T>
	// すでにあるunique_ptrを元に追加するパターン。あらかじめ初期化処理を読んだ物を渡すようにしてください。
	T* AddGameObject(std::unique_ptr<T>&& object, int layerNum, Transform trans = Transform())
	{
		// 型チェック
		if constexpr (!std::is_base_of_v<Object3D, T> && !std::is_base_of_v<Object2D, T>)
		{
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return nullptr; // 型が違う場合はnullptrを返す
		}
		// unique_ptrで受け取っているため初期化処理は実行しているものとみなす

		if constexpr (std::is_base_of_v<Object2D, T>)
		{
			if (layerNum < 0)
			{
				//
			}
			else if (layerNum >= static_cast<int>(m_Objects2D.size()))
			{
				for (int i = static_cast<int>(m_Objects2D.size()); i <= layerNum; i++)
				{
					m_Objects2D.emplace_back(std::list<std::unique_ptr<Object2D>>());
				}
			}
			auto it = m_Objects2D.begin();
			std::advance(it, layerNum);
			it->push_back(std::unique_ptr<Object2D>(std::move(object)));
			return static_cast<T*>(it->back().get());
		}
		else if constexpr (std::is_base_of_v<Object3D, T>)
		{
			if (layerNum < 0)
			{
				//
			}
			else if (layerNum >= static_cast<int>(m_Objects3D.size()))
			{
				for (int i = static_cast<int>(m_Objects3D.size()); i <= layerNum; i++)
				{
					m_Objects3D.emplace_back(std::list<std::unique_ptr<Object3D>>());
				}
			}
			auto it = m_Objects3D.begin();
			std::advance(it, layerNum);
			it->push_back((std::move(object)));
			return static_cast<T*>(it->back().get());
		}
		return nullptr; // ここに来ることはないはず
	}

	template<typename T>
	T* AddSystemObject()
	{
		static_assert(std::is_base_of_v<SystemObject, T>, "Unsupported SystemObject type");
		std::unique_ptr<T> system_obj = std::make_unique<T>();
		system_obj->Init();
		m_SystemObjects.push_back(std::move(system_obj));
		return static_cast<T*>(m_SystemObjects.back().get());
	}

	template<typename T>
	std::unique_ptr<T> GetGameObjectUniquePtr(T* rawPtr)
	{
		static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");

		if constexpr (std::is_base_of_v<Object2D, T>)
		{
			for (auto& layer : m_Objects2D)
			{
				for (auto it = layer.begin(); it != layer.end(); ++it)
				{
					if (it->get() == rawPtr)
					{
						// 型安全にチェックしてから所有権を移す
						if (dynamic_cast<T*>(rawPtr) == nullptr)
						{
							return nullptr; // 型違い: 所有権は移さない
						}
						T* raw = static_cast<T*>(it->release()); // ownership を手放させる
						layer.erase(it);	// コンテナから削除
						return std::unique_ptr<T>(raw);	// 新しい unique_ptr<T> を返す
					}
				}
			}
		}
		else if constexpr (std::is_base_of_v<Object3D, T>)
		{
			for (auto& layer : m_Objects3D)
			{
				for (auto it = layer.begin(); it != layer.end(); ++it)
				{
					if (it->get() == rawPtr)
					{
						if (dynamic_cast<T*>(rawPtr) == nullptr)
						{
							return nullptr;
						}
						T* raw = static_cast<T*>(it->release());
						layer.erase(it);
						return std::unique_ptr<T>(raw);
					}
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetGameObject()
	{
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr;
					}
				}
			}
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						return ptr;
					}
				}
			}
		}
		else
		{
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return nullptr;
		}
		return nullptr;
	}

	template <typename T>
	std::list<T*> GetGameObjects()
	{
		if constexpr (std::is_base_of_v<Object3D, T>)
		{
			std::list<T*> result;
			for (auto& objects3d : m_Objects3D)
			{
				for (auto& gameObject : objects3d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr);
					}
				}
			}
			return result;
		}
		else if constexpr (std::is_base_of_v<Object2D, T>)
		{
			std::list<T*> result;
			for (auto& objects2d : m_Objects2D)
			{
				for (auto& gameObject : objects2d)
				{
					if (auto ptr = dynamic_cast<T*>(gameObject.get()))
					{
						result.push_back(ptr);
					}
				}
			}
			return result;
		}
		else
		{
			static_assert(std::is_base_of_v<Object3D, T> || std::is_base_of_v<Object2D, T>, "Unsupported GameObject type");
			return std::list<T*>();
		}
	}

	template<typename T>
	T* GetSystemObject()
	{
		for (auto& system : m_SystemObjects)
		{
			if (auto ptr = dynamic_cast<T*>(system.get()))
			{
				return ptr;
			}
		}
		return nullptr;
	}

	// タグを使ってGameObjectを取得
	GameObject* GetGameObjectByTag(const std::string& tag);
	// タグを使ってGameObjectのリストを取得
	std::list<GameObject*> GetGameObjectsByTag(const std::string& tag);

protected:
	void DeleteGameObject();
	void UpdateStateObject();
	void UpdateStateObjectByTag(const std::string& tag);
	void UpdateStateObjectByTags(const std::list<std::string>& tags);
	void DrawStateObject();
	void DrawStateObjectByTag(const std::string& tag);
	void DrawStateObjectByTags(const std::list<std::string>& tags);

private:
	std::list<std::list<std::unique_ptr<Object3D>>> m_Objects3D;
	std::list<std::list<std::unique_ptr<Object2D>>> m_Objects2D;
	std::list<std::unique_ptr<SystemObject>> m_SystemObjects; // システムオブジェクトを保存するリスト
};