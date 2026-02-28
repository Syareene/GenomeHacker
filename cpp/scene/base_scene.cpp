#include "main.h"
#include "scene/base_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "object/camera.h"
#include "object/panel.h"
#include "collider/collision.h"

Scene::~Scene() = default;

// プロトタイプ宣言
GameObject* FindGameObjectByTagRecursive(GameObject* obj, const std::string& tag);
void FindGameObjectsByTagRecursive(GameObject* obj, const std::string& tag, std::list<GameObject*>& result);

std::deque<std::unique_ptr<ISystemObjectManager>> Scene::m_GlobalSystemObjects;
unsigned int Scene::m_ObjectIDCounter = 0;

void Scene::DeleteGameObject()
{
	// 不要になった3dオブジェクトを削除
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		// 不要になった GameObject を削除
		objects3d->RemoveDestroyedObjects();
	}

	// 不要になった2dオブジェクトを削除
	for (auto& objects2d : m_Objects2D)
	{
		if(!objects2d)
		{
			continue;
		}
		// 不要になった GameObject を削除
		objects2d->RemoveDestroyedObjects();
	}
}

void Scene::DeleteAllGameObject()
{
	// 3dオブジェクトの全削除処理
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->RemoveAllObjects();
	}
	// 2dオブジェクトの全削除処理
	for (auto& objects2d : m_Objects2D)
	{
		if(!objects2d)
		{
			continue;
		}
		objects2d->RemoveAllObjects();
	}
	// システムオブジェクトの全削除処理
	for (auto& systemObject : m_SystemObjects)
	{
		if(!systemObject)
		{
			continue;
		}
		systemObject->RemoveAllObjects();
	}
}

void Scene::Init()
{

}


void Scene::Uninit()
{
	// 3dオブジェクトの解放
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->Uninit();
	}
	m_Objects3D.clear();

	// 2dオブジェクトの解放
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->Uninit();
	}

	// システムオブジェクトの解放
	for (auto& systemObject : m_SystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->Uninit();
	}
}

void Scene::UpdateGPUData()
{
	// 3dオブジェクトのGPUデータ更新
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->UpdateGPUData();
	}
	// 2dオブジェクトのGPUデータ更新
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->UpdateGPUData();
	}

	// stateのオブジェクトも更新
	m_StateManager.UpdateGPUData();
}

void Scene::DrawObjectsByQueue()
{
	// キューの作成
	std::vector<RenderQueueData> renderQueue;
	// リクエスト数の予測（パフォーマンス向上のため）
	renderQueue.reserve(1024);

	// 各マネージャーからリクエストを収集
	for (auto& manager : m_Objects3D)
	{
		if (manager) manager->SubmitDrawRequests(renderQueue, GetDrawTargetTags());
	}

	for (auto& manager : m_Objects2D)
	{
		if (manager) manager->SubmitDrawRequests(renderQueue, GetDrawTargetTags());
	}

	// Stateのオブジェクトに対してもキューを走らせる
	m_StateManager.SubmitDrawRequests(renderQueue);


	// レイヤー順にソートする
	std::sort(renderQueue.begin(), renderQueue.end());

	// 実行
	for (const auto& req : renderQueue)
	{
		// インスタンシングレンダリングならまとめてスタックに積まれている
		// 対応していない場合は個別に積まれているため個別に関数が呼ばれる
		req.DrawCall();
	}
}

void Scene::FlushPendingObjects()
{
	// 3dオブジェクトの保留中オブジェクトをフラッシュ
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->FlushPendingObjects();
	}
	ObjectManager<Panel>* panelManager = nullptr;

	// 2dオブジェクトの保留中オブジェクトをフラッシュ
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		ObjectManager<Panel>* manager = dynamic_cast<ObjectManager<Panel>*>(objects2d.get());
		if (manager)
		{
			panelManager = manager;
		}
		objects2d->FlushPendingObjects();
	}

	// パネル内にもmanagerがあるためそちらもフラッシュ処理

	// これパネル継承したやつの場合実行されんね
	if (panelManager)
	{
		for (auto& panelObj : panelManager->GetGameObjects())
		{
			panelObj.FlushPendingObjects();
		}
	}
}

void Scene::UpdateObject()
{
	// グローバルシステムオブジェクトの更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->Update();
	}

	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->Update();
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->Update();
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
	// 待機オブジェクトの反映
	FlushPendingObjects();
}
void Scene::UpdateObjectByTag(const std::string& tag)
{
	// グローバルなシステムオブジェクトの更新
	for(auto& systemObject : m_GlobalSystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->Update();
	}

	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->UpdateObjectByTag(tag);
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		if (!objects3d)
		{
			continue;
		}
		objects3d->UpdateObjectByTag(tag);
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->UpdateObjectByTag(tag);
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
	// 待機オブジェクトの反映
	FlushPendingObjects();
}

void Scene::UpdateObjectByTags(const std::list<std::string>& tags)
{
	// グローバルなシステムオブジェクトの更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject.get()->Update();
	}

	// システムオブジェクトはSystemタグがあれば更新
	for (auto& systemObject : m_SystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->UpdateObjectByTags(tags);
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		if (!objects3d)
		{
			continue;
		}
		objects3d->UpdateObjectByTags(tags);
	}
	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		if(!objects2d)
		{
			continue;
		}
		objects2d->UpdateObjectByTags(tags);
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
	// 待機オブジェクトの反映
	FlushPendingObjects();
}

void Scene::DrawObject()
{
	// 描画順を揃えるためにソートする(listのアルゴリズム使用)->3dobjだけでいいからそこは工夫したいね
	// 本当はカメラのz方向の距離でソートしないとダメ
	// m_GameObject[1].sort([&](GameObject* a, GameObject* b)
	//{
		// ここで描画順を決める
		// 例えば、Z座標が小さい方を先に描画するなど
	//	return a->GetDistance(cameraPosition) > b->GetDistance(cameraPosition);
	//});

	// 3dオブジェクトの描画

	// 3dオブジェクトの描画
	for(auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->Draw();
	}

	// 3dが描画し終わったので2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->Draw();
	}
}

void Scene::DrawObjectByTag(const std::string& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->DrawObjectByTag(tag);
	}

	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		if(!objects2d)
		{
			continue;
		}
		objects2d->DrawObjectByTag(tag);
	}
}

void Scene::DrawObjectByTags(const std::list<std::string>& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		objects3d->DrawObjectByTags(tag);
	}
	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		if (!objects2d)
		{
			continue;
		}
		objects2d->DrawObjectByTags(tag);
	}
}

GameObject* Scene::GetGameObjectByTag(const std::string& tag)
{
	GameObject* temp = nullptr;
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		temp = objects3d->GetObjectByTag(tag);
		if (temp)
		{
			return temp;
		}
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{	
		if(!objects2d)
		{
			continue;
		}
		temp = objects2d->GetObjectByTag(tag);
		if (temp)
		{
			return temp;
		}
	}
	return nullptr; // 見つからなかったらnullptrを返す
}

std::list<GameObject*> Scene::GetGameObjectsByTag(const std::string& tag)
{
	std::list<GameObject*> result;
	std::list<GameObject*> temp;
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		if(!objects3d)
		{
			continue;
		}
		temp = objects3d->GetObjectsByTag(tag);
		if (!temp.empty())
		{
			result.splice(result.end(), temp);
		}
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{
		if(!objects2d)
		{
			continue;
		}
		temp = objects2d->GetObjectsByTag(tag);
		if (!temp.empty())
		{
			result.splice(result.end(), temp);
		}
	}
	return result; // タグを持つオブジェクトのリストを返す
}

void Scene::UpdateFinal()
{
	// グローバルなシステムオブジェクトの最終更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject.get()->UpdateFinal();
	}

	// systemオブジェクトの最終更新
	for (auto& systemObject : m_SystemObjects)
	{
		if (!systemObject)
		{
			continue;
		}
		systemObject->UpdateFinal();
	}
}