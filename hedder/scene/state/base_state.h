#pragma once

#include <list>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include "object/game_object.h"
#include "object/system_object.h"
#include "object/3d_object.h"
#include "object/2d_object.h"
#include "manager/object_manager.h"

// concept(c++20~)はクラス外で定義する必要あり
template<typename T>
concept SupportedGameObject = std::is_base_of_v<Object2D, T> || std::is_base_of_v<Object3D, T>;
template<typename T>
concept SystemObj = std::is_base_of_v<SystemObject, T>;

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

	// 2d版リザーブ関数
	template <typename ObjectType>
	void ReserveObject(size_t capacity) requires std::is_base_of_v<Object2D, ObjectType>
	{
		const int typeId = getTypeId<ObjectType>();
		// サイズが足りない場合は拡張
		if ((int)m_Objects2D.size() <= typeId)
		{
			m_Objects2D.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if (!m_Objects2D[typeId])
		{
			m_Objects2D[typeId] = std::make_unique<ObjectManager<ObjectType>>();
		}
		// キャストしてリザーブ実行
		auto manager = static_cast<ObjectManager<ObjectType>*>(m_Objects2D[typeId].get());
		manager->Reserve(capacity);
	}

	// 3d版リザーブ関数
	template <typename ObjectType>
	void ReserveObject(size_t capacity) requires std::is_base_of_v<Object3D, ObjectType>
	{
		const int typeId = getTypeId<ObjectType>();
		// サイズが足りない場合は拡張
		if ((int)m_Objects3D.size() <= typeId)
		{
			m_Objects3D.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if (!m_Objects3D[typeId])
		{
			m_Objects3D[typeId] = std::make_unique<ObjectManager<ObjectType>>();
		}
		// キャストしてリザーブ実行
		auto manager = static_cast<ObjectManager<ObjectType>*>(m_Objects3D[typeId].get());
		manager->Reserve(capacity);
	}

	// システム版リザーブ関数
	template <typename ObjectType>
	void ReserveSystemObject(size_t capacity) requires std::is_base_of_v<SystemObject, ObjectType>
	{
		const int typeId = getTypeId<ObjectType>();
		// シーン内でのシステムオブジェクトとしてリザーブする
		// サイズが足りない場合は拡張
		if ((int)m_SystemObjects.size() <= typeId)
		{
			m_SystemObjects.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if (!m_SystemObjects[typeId])
		{
			m_SystemObjects[typeId] = std::make_unique<SystemObjectManager<ObjectType>>();
		}
		// キャストしてリザーブ実行
		auto manager = static_cast<SystemObjectManager<ObjectType>*>(m_SystemObjects[typeId].get());
		manager->Reserve(capacity);
	}

	// 2dオブジェクト追加関数
	template<typename T, typename... Args>
	T* AddGameObject(int layerNum, Args&&... args) requires std::is_base_of_v<Object2D, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		// サイズが足りない場合は拡張する
		if ((int)m_Objects2D.size() <= typeId)
		{
			m_Objects2D.resize(typeId + 1);
		}

		// まだマネージャーがない場合は作成
		if (!m_Objects2D[typeId])
		{
			m_Objects2D[typeId] = std::make_unique<ObjectManager<T>>();
		}

		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(layerNum, GetNextObjectID(), std::forward<Args>(args)...);
	}

	// 3dオブジェクト追加関数
	template<typename T, typename... Args>
	T* AddGameObject(int layerNum, Args&&... args) requires std::is_base_of_v<Object3D, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		// サイズが足りない場合は拡張する
		if ((int)m_Objects3D.size() <= typeId)
		{
			m_Objects3D.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if (!m_Objects3D[typeId])
		{
			m_Objects3D[typeId] = std::make_unique<ObjectManager<T>>();
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects3D[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(layerNum, GetNextObjectID(), std::forward<Args>(args)...);
	}

	// systemオブジェクト追加関数
	template<typename T, typename... Args>
	T* AddSystemObject(Args&&... args) requires std::is_base_of_v<SystemObject, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		// シーン内でのシステムオブジェクトとして追加する
		// サイズが足りない場合は拡張する
		if ((int)m_SystemObjects.size() <= typeId)
		{
			m_SystemObjects.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if (!m_SystemObjects[typeId])
		{
			m_SystemObjects[typeId] = std::make_unique<SystemObjectManager<T>>();
		}
		auto manager = static_cast<SystemObjectManager<T>*>(m_SystemObjects[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(std::forward<Args>(args)...);
	}

	// 2dオブジェクトを取得する関数
	template <typename T>
	T* GetGameObject() requires std::is_base_of_v<Object2D, T>
	{
		const int typeId = getTypeId<T>();
		if ((int)m_Objects2D.size() <= typeId || !m_Objects2D[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		return manager->GetGameObject();
	}

	// 2dオブジェクトをまとめて取得する関数
	template <typename T>
	std::vector<T>& GetGameObjects() requires std::is_base_of_v<Object2D, T>
	{
		const int typeId = getTypeId<T>();
		if ((int)m_Objects2D.size() <= typeId || !m_Objects2D[typeId])
		{
			static std::vector<T> empty; // 空のベクターを返す
			return empty;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		return manager->GetGameObjects();
	}

	// 3dオブジェクトを取得する関数
	template <typename T>
	T* GetGameObject() requires std::is_base_of_v<Object3D, T>
	{
		const int typeId = getTypeId<T>();
		if ((int)m_Objects3D.size() <= typeId || !m_Objects3D[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects3D[typeId].get());
		return manager->GetGameObject();
	}

	// 3dオブジェクトをまとめて取得する関数
	template <typename T>
	std::vector<T>& GetGameObjects() requires std::is_base_of_v<Object3D, T>
	{
		const int typeId = getTypeId<T>();
		if ((int)m_Objects3D.size() <= typeId || !m_Objects3D[typeId])
		{
			static std::vector<T> empty; // 空のベクターを返す
			return empty;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects3D[typeId].get());
		return manager->GetGameObjects();
	}

	// システムオブジェクト取得関数
	template <typename T>
	T* GetSystemObject() requires std::is_base_of_v<SystemObject, T>
	{
		const int typeId = getTypeId<T>();
		// シーン内に存在するシステムオブジェクトから探す
		if ((int)m_SystemObjects.size() <= typeId || !m_SystemObjects[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<SystemObjectManager<T>*>(m_SystemObjects[typeId].get());
		return manager->GetSystemObject();
	}

	// idを用いてGameObjectを取得
	template <typename T>
	T* GetGameObjectById(unsigned int id) requires std::is_base_of_v<Object2D, T>
	{
		// id取得
		const int typeId = getTypeId<T>();
		if ((int)m_Objects2D.size() <= typeId || !m_Objects2D[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		return manager->GetObjectById(id);
	}

	// idを用いてGameObjectを取得
	template <typename T>
	T* GetGameObjectById(unsigned int id) requires std::is_base_of_v<Object3D, T>
	{
		// id取得
		const int typeId = getTypeId<T>();
		if ((int)m_Objects3D.size() <= typeId || !m_Objects3D[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects3D[typeId].get());
		return manager->GetObjectById(id);
	}

	// タグを使ってGameObjectを取得
	GameObject* GetGameObjectByTag(const std::string& tag);
	// タグを使ってGameObjectのリストを取得
	std::list<GameObject*> GetGameObjectsByTag(const std::string& tag);

	template <typename T>
	inline int getTypeId() requires std::is_base_of_v<Object2D, T>
	{
		return Object2D::getTypeId<T>();
	}

	template <typename T>
	inline int getTypeId() requires std::is_base_of_v<Object3D, T>
	{
		return Object3D::getTypeId<T>();
	}

	template <typename T>
	inline int getTypeId() requires std::is_base_of_v<SystemObject, T>
	{
		return SystemObject::getTypeId<T>();
	}

protected:
	inline bool IsInitialized() const { return m_IsInitialized; }
	inline void SetIsInitialized(const bool& isInitialized) { m_IsInitialized = isInitialized; }
	void DeleteGameObject();
	void DeleteAllGameObject();
	void UpdateStateObject();
	void UpdateStateObjectByTag(const std::string& tag);
	void UpdateStateObjectByTags(const std::list<std::string>& tags);
	void DrawStateObject();
	void DrawStateObjectByTag(const std::string& tag);
	void DrawStateObjectByTags(const std::list<std::string>& tags);

private:
	static unsigned int GetNextObjectID()
	{
		// 現在値が2147483647に達したら0に戻す
		if (m_ObjectIDCounter == INT_MAX)
		{
			m_ObjectIDCounter = 0;
		}
		return m_ObjectIDCounter++;
	}
	static unsigned int m_ObjectIDCounter;
	bool m_IsInitialized = false;
	std::vector<std::unique_ptr<IGameObjectManager>> m_Objects3D;
	std::vector<std::unique_ptr<IGameObjectManager>> m_Objects2D;
	std::vector<std::unique_ptr<ISystemObjectManager>> m_SystemObjects;
};
