#include "main.h"
#include "object/panel.h"

unsigned int Panel::m_ObjectIDCounter = 0;

void Panel::Init()
{
	// パネルの初期化処理

	// 子オブジェクトの初期化
	//for (auto& child : m_ChildObjects)
	//{
	//	
	//}
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
	// 待機オブジェクトの反映
	FlushPendingObjects();
}

void Panel::UpdateGPUData(InstanceBufferData& data)
{
	// パネルのGPUデータ更新処理
	//UpdateGPUData(data);
	// 子オブジェクトのGPUデータ更新
	for (auto& child : m_ChildObjects)
	{
		if (!child)
		{
			continue;
		}
		child->UpdateGPUData();
	}
}

void Panel::SetPipelineState()
{
	// パネルのパイプラインステート設定(一旦特になし)
}

void Panel::SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue, std::deque<std::string> tags)
{
	// 子オブジェクトの描画要求提出
	for (auto& child : m_ChildObjects)
	{
		if (!child)
		{
			continue;
		}
		child->SubmitDrawRequests(renderQueue, tags);
	}
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

void Panel::FlushPendingObjects()
{
	// 子オブジェクトの保留中オブジェクトをフラッシュ
	for (auto& child : m_ChildObjects)
	{
		if (!child)
		{
			continue;
		}
		child->FlushPendingObjects();
	}
}