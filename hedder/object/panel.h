#pragma once

#include "object/2d_object.h"
#include <memory>


// こーれパネルの使用どうしよ。2d限定とか?
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

	// get系は軒並みbase_sceneにあるものを作りたいね
	// というかパネルにするとbase_sceneのgetobjectから取得できなくなる気がする、、タグ版は実装済みだがTから取得するやつは未実装。
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

	// ここに対してadd_objectする場合、scene自体にadd_objectし、その参照をここで持つようにする?
	// その場合処理順が親->子にならない可能性があるのでそこだけが懸念点。

	// ここmoveないとエラー出る->inputの形式変えたほうがいいかも
	Object2D* AddChildObject(std::unique_ptr<Object2D> child) 
	{ 
		m_ChildObjects.push_back(std::move(child));
		return m_ChildObjects.back().get(); // 追加した子オブジェクトのポインタを返す
	}

	/*
	template<typename T>
	T* AddChildObject()
	{
		m_ChildObjects.push_back(std::make_unique<T>());
		return static_cast<T*>(m_ChildObjects.back().get());
	}
	*/
	
};