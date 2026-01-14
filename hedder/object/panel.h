#pragma once

#include "object/2d_object.h"
#include "manager/object_manager.h"
#include <vector>
#include <memory>

// concept定義
template<typename T>
concept PanelSupportedGameObject = std::is_base_of_v<Object2D, T>;

// 現状パネルは2d限定
class Panel : public Object2D
{
private:
	std::vector<std::unique_ptr<IGameObjectManager>> m_ChildObjects; // 子オブジェクトのリスト
	static unsigned int m_ObjectIDCounter; // シーンのとはまた異なるカウンター->本当にこれでいいかをちょっと考えるべきかも
	static unsigned int GetNextObjectID()
	{
		// 現在値が2147483647に達したら0に戻す
		if (m_ObjectIDCounter == INT_MAX)
		{
			m_ObjectIDCounter = 0;
		}
		return m_ObjectIDCounter++;
	}
	// このとき子オブジェクトからdestoryとかが呼ばれた際にこのリストからちゃんと消えるか問題はあるよねぇ、、->updateのところに消す処理書いたけどunique_ptrにしてるので変える必要あり
public:
	Panel() = default;
	~Panel() override = default;
	Panel(Panel&&) noexcept = default;
	Panel& operator=(Panel&&) noexcept = default;

	virtual void Init();
	void Uninit() override;
	void Update() override;
	// sceneみたいに後付pushにするかちょい悩む
	void Draw() override;

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

	// タグ検索のオーバーライド
	GameObject* FindObjectByTag(const std::string& tag) override
	{
		// 自身をチェック
		for(auto& t : GetTagList())
		{
			if(t == tag)
			{
				return this;
			}
		}

		// 子オブジェクトを再帰的にチェック
		for(auto& child : m_ChildObjects)
		{
			for(auto& obj : child)
			{
				GameObject* found = obj->FindObjectByTag(tag);
				if(found)
				{
					return found;
				}
			}
		}
		// 該当なし
		return nullptr;
	}
	// 複数取得のオーバーライド
	void FindObjectsByTag(const std::string& tag, std::list<GameObject*>& result) override
	{
		// 自身をチェック
		for(auto& t : GetTagList())
		{
			if(t == tag)
			{
				result.push_back(this);
				break; // 見つけたらループを抜ける
			}
		}
		// 子オブジェクトを再帰的にチェック
		for(auto& child : m_ChildObjects)
		{
			for(auto& obj : child)
			{
				obj->FindObjectsByTag(tag, result);
			}
		}
	}

	// これref取るのめんどいから削除かな
	std::list<std::list<std::unique_ptr<Object2D>>>& GetAllChildObjects()
	{
		return m_ChildObjects;
	}

	std::list<Object2D*> GetChildObjects(int index)
	{
		// index番目のレイヤーの子オブジェクトを取得
		std::list<Object2D*> objects;
		if (index < 0 || index >= static_cast<int>(m_ChildObjects.size()))
		{
			return objects; // 範囲外なら空のリストを返す
		}
		auto it = m_ChildObjects.begin();
		std::advance(it, index);
		for (const auto& child : *it)
		{
			objects.push_back(child.get());
		}
		return objects;
	}

	template <typename T>
	T* GetChildObjectByType()
	{
		const int typeId = getTypeId<T>();
		if ((int)m_Objects2D.size() <= typeId || !m_Objects2D[typeId])
		{
			return nullptr;
		}
		auto manager = static_cast<ObjectManager<T>*>(m_Objects2D[typeId].get());
		return manager->GetGameObject();
	}

	template <typename T>
	std::list<T*> GetChildObjectsByType()
	{
		std::list<T*> objects;
		for (const auto& layer : m_ChildObjects)
		{
			for (const auto& child : layer)
			{
				if (child && dynamic_cast<T*>(child.get()))
				{
					objects.push_back(dynamic_cast<T*>(child.get()));
				}
			}
		}
		return objects;
	}

	template <PanelSupportedGameObject T>
	T* AddChildObject(int layerNum, Transform trans = Transform())
	{
		// 中でインスタンスを作る
		auto child = std::make_unique<T>();
		child->Init(trans); // 初期化実行
		T* childPtr = child.get();

		// layerNumとコンテナのサイズを比べる
		if (layerNum < 0)
		{
			// ToDo: handle error
		}
		else if (layerNum >= static_cast<int>(m_ChildObjects.size()))
		{
			// layerNumがコンテナのサイズ以上ならその数まで空の要素を追加する
			for (int i = static_cast<int>(m_ChildObjects.size()); i <= layerNum; i++)
			{
				// 追加
				m_ChildObjects.emplace_back(std::list<std::unique_ptr<Object2D>>());
			}
		}
		// layerNum分iteratorを進める
		auto it = m_ChildObjects.begin();
		std::advance(it, layerNum);
		// layerNumの位置に追加
		it->push_back(std::move(child));
		// スマポで管理しつつも生ポインタで返すように
		return childPtr; // 追加した子オブジェクトのポインタを返す
	}

	void DeleteChildObject(void)
	{
		// destoryフラグが立っているオブジェクトを削除
		for (auto& layer : m_ChildObjects)
		{
			layer.remove_if([](const std::unique_ptr<Object2D>& obj) {
				// objがnullptrでないことを確認し、Destroyメソッドを呼び出す
				return obj && obj->Destroy();
			});
		}
	}

	// すべての子オブジェクトを削除
	void DeleteAllChildObject()
	{

	}
	
};