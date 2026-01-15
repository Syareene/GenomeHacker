#include "main.h"
#include "object/panel.h"

unsigned int Panel::m_ObjectIDCounter = 0;

void Panel::Init()
{
	// パネルの初期化処理

	// 子オブジェクトの初期化
	for (auto& child : m_ChildObjects)
	{
		
	}
}

void Panel::Uninit()
{
	// パネルの終了処理
	Object2D::Uninit();

	// 子オブジェクトの終了
	for (auto& child : m_ChildObjects)
	{
		if(!child)
		{
			continue;
		}
		child->Uninit();
	}

	// リストクリア
	m_ChildObjects.clear();
}

void Panel::Update()
{
	// パネルの更新処理
	Object2D::Update();
	// 子オブジェクトの更新
	for (auto& child : m_ChildObjects)
	{
		if(!child)
		{
			continue;
		}
		child->Update();
	}

	// 不要な子オブジェクトの削除処理
	DeleteChildObject();
}

void Panel::Draw()
{
	// パネルの描画処理
	Object2D::Draw();
	// 子オブジェクトの描画
	for (auto& child : m_ChildObjects)
	{
		if (!child)
		{
			continue;
		}
		child->Draw();
	}
}