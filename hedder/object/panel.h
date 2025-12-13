#pragma once

#include "object/2d_object.h"
#include <memory>

// concept定義
template<typename T>
concept PanelSupportedGameObject = std::is_base_of_v<Object2D, T>;

// 現状パネルは2d限定
class Panel : public Object2D
{
private:

	// 描画順だけどうするかだねぇ各オブジェクトの
	std::list<std::unique_ptr<Object2D>> m_ChildObjects; // 子オブジェクトのリスト
	// このとき子オブジェクトからdestoryとかが呼ばれた際にこのリストからちゃんと消えるか問題はあるよねぇ、、->updateのところに消す処理書いたけどunique_ptrにしてないので変える必要あり
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	std::list<Object2D*> GetChildObjects()
	{
		// 中身の要素を全列挙しポインタで返す
		std::list<Object2D*> childObjects;
		for (const auto& child : m_ChildObjects)
		{
			if (child)
			{
				childObjects.push_back(child.get()); // unique_ptrから生ポインタを取得
			}
		}
		return childObjects;
	}

	template <typename T>
	T* GetChildObjectByType()
	{
		for (const auto& child : m_ChildObjects)
		{
			if (child && dynamic_cast<T*>(child.get()))
			{
				return dynamic_cast<T*>(child.get());
			}
		}
		return nullptr; // 見つからなかった場合
	}

	template <typename T>
	std::list<T*> GetChildObjectsByType()
	{
		std::list<T*> objects;
		for (const auto& child : m_ChildObjects)
		{
			if (child && dynamic_cast<T*>(child.get()))
			{
				objects.push_back(dynamic_cast<T*>(child.get()));
			}
		}
		return objects;
	}

	template <PanelSupportedGameObject T>
	T* AddChildObject(Transform trans = Transform())
	{
		// 中でインスタンスを作る
		auto child = std::make_unique<T>();
		child->Init(trans); // 初期化実行
		T* childPtr = child.get();
		m_ChildObjects.push_back(std::move(child));
		return childPtr; // 追加した子オブジェクトのポインタを返す
	}

	void DeleteChildObject(void)
	{
		// destoryフラグが立っているオブジェクトを削除
		m_ChildObjects.remove_if([](const std::unique_ptr<Object2D>& obj) {
			// objがnullptrでないことを確認し、Destroyメソッドを呼び出す
			return obj && obj->Destroy();
		});
	}
	
};