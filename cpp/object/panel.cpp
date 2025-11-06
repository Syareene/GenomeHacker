#include "main.h"
#include "object/panel.h"

void Panel::Init(Transform trans)
{
	SetTransform(trans);
	// パネルの初期化処理
	GameObject::Init();

	// 子オブジェクトの初期化
	for (auto& child : m_ChildObjects)
	{
		child.get()->Init();
	}
}

void Panel::Uninit()
{
	// パネルの終了処理
	GameObject::Uninit();

	// 子オブジェクトの終了
	for (auto& child : m_ChildObjects)
	{
		child.get()->Uninit();
	}
}

void Panel::Update()
{
	// パネルの更新処理
	GameObject::Update();
	// 子オブジェクトの更新
	for (auto& child : m_ChildObjects)
	{
		child.get()->Update();
	}

	// 不要な子オブジェクトの削除処理
	m_ChildObjects.remove_if([](std::unique_ptr<GameObject>& obj) {
		return obj && obj->Destroy(); // 子オブジェクトが削除予約されている場合
		});
}

void Panel::Draw()
{
	// パネルの描画処理
	GameObject::Draw();
	// 子オブジェクトの描画
	for (auto& child : m_ChildObjects)
	{
		child.get()->Draw();
	}
}