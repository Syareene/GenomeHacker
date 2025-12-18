#include "main.h"
#include "object/panel.h"

void Panel::Init(Transform trans)
{
	// パネルの初期化処理
	Object2D::Init(trans);

	// 子オブジェクトの初期化
	for (auto& layer : m_ChildObjects)
	{
		for (auto& child : layer)
		{
			child.get()->Init();
		}
	}
}

void Panel::Uninit()
{
	// パネルの終了処理
	Object2D::Uninit();

	// 子オブジェクトの終了
	for (auto& layer : m_ChildObjects)
	{
		for (auto& child : layer)
		{
			child.get()->Uninit();
		}
	}
}

void Panel::Update()
{
	// パネルの更新処理
	Object2D::Update();
	// 子オブジェクトの更新
	for (auto& layer : m_ChildObjects)
	{
		for (auto& child : layer)
		{
			child.get()->Update();
		}
	}

	// 不要な子オブジェクトの削除処理
	DeleteChildObject();
}

void Panel::Draw()
{
	// パネルの描画処理
	Object2D::Draw();
	// 子オブジェクトの描画
	for (auto& layer : m_ChildObjects)
	{
		for (auto& child : layer)
		{
			child.get()->Draw();
		}
	}
}