#pragma once

#include "object/game_object.h"
#include <memory>


// こーれパネルの使用どうしよ。2d限定とか?
class Panel : public GameObject
{
private:

	// 描画順だけどうするかだねぇ各オブジェクトの
	std::list<std::unique_ptr<GameObject>> m_ChildObjects; // 子オブジェクトのリスト
	// このとき子オブジェクトからdestoryとかが呼ばれた際にこのリストからちゃんと消えるか問題はあるよねぇ、、->updateのところに消す処理書いたけどunique_ptrにしてないので変える必要あり
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// get系は軒並みbase_sceneにあるものを作りたいね
	// というかパネルにするとbase_sceneのgetobjectから取得できなくなる気がする、、これ問題か????
	std::list<GameObject*> GetChildObjects()
	{
		// 中身の要素を全列挙しポインタで返す
		std::list<GameObject*> childObjects;
		for (const auto& child : m_ChildObjects)
		{
			if (child)
			{
				childObjects.push_back(child.get()); // unique_ptrから生ポインタを取得
			}
		}
		return childObjects;
	}

	// ここに対してadd_objectする場合、scene自体にadd_objectし、その参照をここで持つようにする?
	// その場合処理順が親->子にならない可能性があるのでそこだけが懸念点。

	// ここmoveないとエラー出る->inputの形式変えたほうがいいかも
	GameObject* AddChildObject(std::unique_ptr<GameObject> child) 
	{ 
		m_ChildObjects.push_back(std::move(child));
		return m_ChildObjects.back().get(); // 追加した子オブジェクトのポインタを返す
	}
};