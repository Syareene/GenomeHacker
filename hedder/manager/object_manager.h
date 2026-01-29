#pragma once

#include <vector>
#include <memory>
#include <algorithm>
// debugmsg用
#include <typeinfo>
#include <string>
#include "Windows.h"

#include "object/game_object.h"

class Panel; // 前方宣言


// ObjectManagerのインターフェースとなる基底クラス
class IObjectManager
{
public:
	virtual ~IObjectManager() = default;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void UpdateObjectByTag(const std::string& tag) = 0;
	virtual void UpdateObjectByTags(const std::list<std::string>& tags) = 0;
	// 破棄関数
	virtual void RemoveDestroyedObjects() = 0;
	virtual void RemoveAllObjects() = 0;
	// リザーブ関数
	virtual void Reserve(size_t capacity) = 0;
};

// ゲームオブジェクト用の機能を定義するインターフェースクラス
class IGameObjectManager : public IObjectManager
{
public:
	virtual GameObject* GetObjectByTag(const std::string& tag) = 0;
	virtual std::list<GameObject*> GetObjectsByTag(const std::string& tag) = 0;
	virtual void FlushPendingObjects() = 0;
	virtual void Draw() = 0;
	virtual void DrawObjectByTag(const std::string& tag) = 0;
	virtual void DrawObjectByTags(const std::list<std::string>& tags) = 0;
};

// システムオブジェクト用の機能を定義するインターフェースクラス
class ISystemObjectManager : public IObjectManager
{
public:
	virtual void UpdateFinal() = 0;
};

	

template <typename ObjectType>
class ObjectManager : public IGameObjectManager
{
public:
	ObjectManager()
	{
		m_Objects.reserve(ObjectType::MAX_OBJECTS);
	}

	// タグを用いた単体取得
	GameObject* GetObjectByTag(const std::string& tag) override
	{
		for (auto& obj : m_Objects)
		{
			GameObject* found = obj.FindObjectByTag(tag);
			if (found)
			{
				return found;
			}
		}
		return nullptr;
	}
	
	// タグを用いた複数取得
	std::list<GameObject*> GetObjectsByTag(const std::string& tag) override
	{
		std::list<GameObject*> result;
		for (auto& obj : m_Objects)
		{
			obj.FindObjectsByTag(tag, result);
		}
		return result;
	}

	// idを用いた単体取得
	ObjectType* GetObjectById(const unsigned int& id)
	{
		for (auto& obj : m_Objects)
		{
			if (obj.GetObjectID() == id)
			{
				return &obj;
			}
		}
		return nullptr;
	}

	void RemoveDestroyedObjects() override
	{
		for (auto& object : m_Objects)
		{
			if (object.IsDestroy())
			{
				object.Uninit();
			}
		}

		std::erase_if(m_Objects, [](const ObjectType& obj) 
		{
			return obj.IsDestroy(); // 削除する条件
		});
	}

	void RemoveAllObjects() override
	{
		// 終了処理実行
		for(auto& object : m_Objects)
		{
			object.Uninit();
		}
		// リストから消す
		m_Objects.clear();
	}


	void Reserve(size_t capacity)
	{
		m_Objects.reserve(capacity);
	}

	template<typename... Args>
	// ここで返ったポインタを保存しないように!
	ObjectType* AddObject(int index, unsigned int objId, Args&&... args)
	{
		// 待機リストに追加
		auto& obj = m_PendingObjects.emplace_back();
		obj.SetObjectID(objId);
		obj.Init(std::forward<Args>(args)...);
		return &obj;
	}

	void FlushPendingObjects() override
	{
		if(m_PendingObjects.empty())
		{
			return; // 保留中のオブジェクトがなければ何もしない
		}
		// 保留中のオブジェクトを本体リストに移動
		for (auto& obj : m_PendingObjects)
		{
			// キャパシティチェック
			if (m_Objects.size() >= m_Objects.capacity())
			{
				// エラーメッセージを作成
				std::string errorMsg = "[Error] ObjectManager capacity exceeded! Target Type: ";
				errorMsg += typeid(ObjectType).name();
				errorMsg += "\nWrite more value on hedder!\n";

				// Visual Studioの出力ウィンドウに表示
				OutputDebugStringA(errorMsg.c_str());

				// 停止
				assert(!"Capacity exceeded! Check Output Window for the object type.");
			}
			m_Objects.emplace_back(std::move(obj));
		}
		m_PendingObjects.clear();
	}

	ObjectType* GetGameObject()
	{
		if (m_Objects.empty())
		{
			return nullptr;
		}
		return &m_Objects[0];
	}

	std::vector<ObjectType>& GetGameObjects()
	{
		return m_Objects;
	}

	// 

	void Init()
	{

	};
	void Uninit() override
	{
		// uninitする
		for(auto& obj : m_Objects)
		{
			obj.Uninit();
		}

		// リストから消す
		m_Objects.clear();
	};
	void Update() override
	{
		// 自身が所有しているオブジェクトを全て更新
		for(auto& obj : m_Objects)
		{
			obj.Update();
		}
	}

	void UpdateObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for(auto& obj : m_Objects)
		{
			if(obj.IsTagAvailable(tag))
			{
				obj.Update();
			}
		}
	}

	void UpdateObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for(auto& obj : m_Objects)
		{
			for(const auto& tag : tags)
			{
				if(obj.IsTagAvailable(tag))
				{
					obj.Update();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}

	void Draw() override
	{
		// とりあえず所有しているオブジェクトを描画
		// 後にインスタンシングレンダリングに対応する形で

		// そうなるとレイヤーはここでvector<vector>にしないとダメやなぁ

		for(auto& obj : m_Objects)
		{
			obj.Draw();
		}
	};

	void DrawObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ描画
		for(auto& obj : m_Objects)
		{
			if(obj.IsTagAvailable(tag))
			{
				obj.Draw();
			}
		}
	}

	void DrawObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ描画
		for(auto& obj : m_Objects)
		{
			for(const auto& tag : tags)
			{
				if(obj.IsTagAvailable(tag))
				{
					obj.Draw();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}


private:
	//int m_Id = 0;
	std::vector<ObjectType> m_Objects;
	std::vector<ObjectType> m_PendingObjects; // 追加待ちオブジェクトのリスト
};

template <typename ObjectType>
class SystemObjectManager : public ISystemObjectManager
{
public:
	SystemObjectManager()
	{
		m_Objects.reserve(ObjectType::MAX_OBJECTS);
	}

	void RemoveDestroyedObjects() override
	{
		// 何もしない
	}

	void RemoveAllObjects() override
	{
		// 終了処理実行
		for (auto& object : m_Objects)
		{
			object.Uninit();
		}
		// リストから消す
		m_Objects.clear();
	}

	void Reserve(size_t capacity)
	{
		m_Objects.reserve(capacity);
	}

	template<typename... Args>
	ObjectType* AddObject(Args&&... args)
	{
		// キャパシティチェック
		if (m_Objects.size() >= m_Objects.capacity())
		{
			// エラーメッセージを作成
			std::string errorMsg = "[Error] ObjectManager capacity exceeded! Target Type: ";
			errorMsg += typeid(ObjectType).name();
			errorMsg += "\nWrite more value on hedder!\n";

			// Visual Studioの出力ウィンドウに表示
			OutputDebugStringA(errorMsg.c_str());

			// 停止
			assert(!"Capacity exceeded! Check Output Window for the object type.");
		}

		// vectorのメモリ上でオブジェクトを構築(emplace_backはc++17から参照を返す)
		auto& obj = m_Objects.emplace_back();
		obj.Init(std::forward<Args>(args)...);

		return &obj;
	}

	ObjectType* GetSystemObject()
	{
		if (m_Objects.empty())
		{
			return nullptr;
		}
		return &m_Objects[0];
	}

	std::vector<ObjectType>& GetSystemObjects()
	{
		return m_Objects;
	}

	void Init()
	{

	}
	void Uninit() override
	{
		for (auto& object : m_Objects)
		{
			object.Uninit();
		}
		m_Objects.clear();
	}

	void Update() override
	{
		for (auto& object : m_Objects)
		{
			object.Update();
		}
	}

	void UpdateObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for (auto& obj : m_Objects)
		{
			if (obj.IsTagAvailable(tag))
			{
				obj.Update();
			}
		}
	}

	void UpdateObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for (auto& obj : m_Objects)
		{
			for (const auto& tag : tags)
			{
				if (obj.IsTagAvailable(tag))
				{
					obj.Update();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}

	void UpdateFinal() override
	{
		for (auto& object : m_Objects)
		{
			object.UpdateFinal();
		}
	}

private:
	//int m_Id = 0;
	std::vector<ObjectType> m_Objects;
};