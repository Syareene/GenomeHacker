#pragma once

#include "object/2d_object.h"
#include <memory>
#include <list>

// concept定義
template<typename T>
concept PanelSupportedGameObject = std::is_base_of_v<Object2D, T>;

// 現状パネルは2d限定
class Panel : public Object2D
{
private:

	// 描画順だけどうするかだねぇ各オブジェクトの
	std::list<std::list<std::unique_ptr<Object2D>>> m_ChildObjects; // 子オブジェクトのリスト
	// このとき子オブジェクトからdestoryとかが呼ばれた際にこのリストからちゃんと消えるか問題はあるよねぇ、、->updateのところに消す処理書いたけどunique_ptrにしてるので変える必要あり
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

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
		for (const auto& layer : m_ChildObjects)
		{
			for (const auto& child : layer)
			{
				if (child && dynamic_cast<T*>(child.get()))
				{
					return dynamic_cast<T*>(child.get());
				}
			}
		}
		return nullptr; // 見つからなかった場合
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
	
};