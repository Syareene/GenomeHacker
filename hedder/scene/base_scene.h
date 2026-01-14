#pragma once

#include <list>
#include <memory>
#include <string>
#include <type_traits>
#include "object/game_object.h"
#include "object/system_object.h"
#include "object/3d_object.h"
#include "object/2d_object.h"
#include "scene/state/state_manager.h" // StateManager in base scene
#include "manager/object_manager.h"

class Object3D; // 前方宣言

// concept(c++20~)はクラス外で定義する必要あり
template<typename T>
concept SupportedGameObject = std::is_base_of_v<Object2D, T> || std::is_base_of_v<Object3D, T>;
template<typename T>
concept SystemObj = std::is_base_of_v<SystemObject, T>;

class Scene
{
public:
	Scene() = default;
	Scene(Scene&& other) noexcept = default;
	virtual ~Scene();

	template <typename T>
	// state変更関数。変更したstateに対して値をセットしたい場合はこの関数の戻り値を使うこと
	T* SetState()
	{
		return m_StateManager.SetState<T>();
	}

	inline State* GetStatePtr() const
	{
		return m_StateManager.GetState();
	}

	template <typename T>
	bool IsState() const
	{
		return m_StateManager.IsCurrentState<T>();
	}

	inline void MoveState()
	{
		m_StateManager.MoveState();
	}

	inline bool IsStateChanged() const
	{
		return m_StateManager.IsStateChanged();
	}

	inline void ResetStateChanged()
	{
		m_StateManager.ResetStateChanged();
	}

	virtual void Init();
	virtual void Uninit();
	virtual void Update() = 0;
	void UpdateObject();
	/// @brief 指定タグを持つオブジェクトのみ更新する
	/// @param tag タグ名
	void UpdateObjectByTag(const std::string& tag);
	void UpdateObjectByTags(const std::list<std::string>& tags);
	virtual void Draw() = 0;
	void DrawObject();
	void DrawObjectByTag(const std::string& tag);
	void DrawObjectByTags(const std::list<std::string>& tags);


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
	void ReserveSystemObject(size_t capacity, bool is_global = false) requires std::is_base_of_v<SystemObject, ObjectType>
	{
		const int typeId = getTypeId<ObjectType>();
		if (is_global)
		{
			// シーン間をまたいで共有されるシステムオブジェクトとしてリザーブする
			// サイズが足りない場合は拡張
			if ((int)m_GlobalSystemObjects.size() <= typeId)
			{
				m_GlobalSystemObjects.resize(typeId + 1);
			}
			// まだマネージャーがない場合は作成
			if (!m_GlobalSystemObjects[typeId])
			{
				m_GlobalSystemObjects[typeId] = std::make_unique<SystemObjectManager<ObjectType>>();
			}
			// キャストしてリザーブ実行
			auto manager = static_cast<SystemObjectManager<ObjectType>*>(m_GlobalSystemObjects[typeId].get());
			manager->Reserve(capacity);
		}
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
	template<typename T>
	T* AddGameObject(int layerNum, Transform trans = Transform()) requires std::is_base_of_v<Object2D, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		// サイズが足りない場合は拡張する
		if((int)m_Objects2D.size() <= typeId)
		{
			m_Objects2D.resize(typeId + 1);
		}

		// まだマネージャーがない場合は作成
		if(!m_Objects2D[typeId])
		{
			m_Objects2D[typeId] = std::make_unique<ObjectManager<T>>();
		}

		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(layerNum, trans);
	}

	// 3dオブジェクト追加関数
	template<typename T>
	T* AddGameObject(int layerNum, Transform trans = Transform()) requires std::is_base_of_v<Object3D, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		// サイズが足りない場合は拡張する
		if((int)m_Objects3D.size() <= typeId)
		{
			m_Objects3D.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if(!m_Objects3D[typeId])
		{
			m_Objects3D[typeId] = std::make_unique<ObjectManager<T>>();
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects3D[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(layerNum, trans);
	}

	// systemオブジェクト追加関数
	template<typename T>
	T* AddSystemObject(bool is_global = false) requires std::is_base_of_v<SystemObject, T>
	{
		// 対象のobjectのidを取得
		const int typeId = getTypeId<T>();
		if (is_global)
		{
			// シーンまたいでも共有されるシステムオブジェクトとして追加する
			// サイズが足りない場合は拡張する
			if ((int)m_GlobalSystemObjects.size() <= typeId)
			{
				m_GlobalSystemObjects.resize(typeId + 1);
			}
			// まだマネージャーがない場合は作成
			if (!m_GlobalSystemObjects[typeId])
			{
				m_GlobalSystemObjects[typeId] = std::make_unique<SystemObjectManager<T>>();
			}
			auto manager = static_cast<SystemObjectManager<T>*>(m_GlobalSystemObjects[typeId].get());
			// 追加したオブジェクトのポインタを返す
			return manager->AddObject(is_global);
		}
		// シーン内でのシステムオブジェクトとして追加する
		// サイズが足りない場合は拡張する
		if((int)m_SystemObjects.size() <= typeId)
		{
			m_SystemObjects.resize(typeId + 1);
		}
		// まだマネージャーがない場合は作成
		if(!m_SystemObjects[typeId])
		{
			m_SystemObjects[typeId] = std::make_unique<SystemObjectManager<T>>();
		}
		auto manager = static_cast<SystemObjectManager<T>*>(m_SystemObjects[typeId].get());
		// 追加したオブジェクトのポインタを返す
		return manager->AddObject(is_global);
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
	T* GetSystemObject(bool is_global = false) requires std::is_base_of_v<SystemObject, T>
	{
		const int typeId = getTypeId<T>();
		if (is_global)
		{
			// staticなシステムオブジェクトから探す
			if ((int)m_GlobalSystemObjects.size() <= typeId || !m_GlobalSystemObjects[typeId])
			{
				return nullptr;
			}
			auto manager = static_cast<SystemObjectManager<T>*>(m_GlobalSystemObjects[typeId].get());
			return manager->GetSystemObject();
		}
		// シーン内に存在するシステムオブジェクトから探す
		if((int)m_SystemObjects.size() <= typeId || !m_SystemObjects[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<SystemObjectManager<T>*>(m_SystemObjects[typeId].get());
		return manager->GetSystemObject();
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
	void DeleteGameObject();
	void DeleteAllGameObject();
	void UpdateFinal(); // システムオブジェクトのUpdateFinalを呼び出す
private:
	std::vector<std::unique_ptr<IGameObjectManager>> m_Objects3D;
	std::vector<std::unique_ptr<IGameObjectManager>> m_Objects2D;
	std::vector<std::unique_ptr<ISystemObjectManager>> m_SystemObjects;
	static std::vector<std::unique_ptr<ISystemObjectManager>> m_GlobalSystemObjects;
	// これ2d/3d/systemそれぞれで作るか?
	// systemの場合はIObjectManagerを通じてdrawを呼ばない?

	StateManager m_StateManager;
};