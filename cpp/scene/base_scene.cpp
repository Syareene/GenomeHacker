#include "main.h"
#include "scene/base_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "object/camera.h"
#include "object/panel.h"

// Define out-of-line destructor
Scene::~Scene() = default;

// プロトタイプ宣言
GameObject* FindGameObjectByTagRecursive(GameObject* obj, const std::string& tag);
void FindGameObjectsByTagRecursive(GameObject* obj, const std::string& tag, std::list<GameObject*>& result);

std::vector<std::unique_ptr<ISystemObjectManager>> Scene::m_GlobalSystemObjects;

void Scene::DeleteGameObject()
{
	// 不要になった3dオブジェクトを削除
	for (auto& objects3d : m_Objects3D)
	{
		// 不要になった GameObject を削除
		objects3d->RemoveDestroyedObjects();
	}

	// 不要になった2dオブジェクトを削除
	for (auto& objects2d : m_Objects2D)
	{
		// 不要になった GameObject を削除
		objects2d->RemoveDestroyedObjects();
	}
}

void Scene::DeleteAllGameObject()
{
	// 3dオブジェクトの全削除処理
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->RemoveAllObjects();
	}
	// 2dオブジェクトの全削除処理
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->RemoveAllObjects();
	}
	// システムオブジェクトの全削除処理
	for (auto& systemObject : m_SystemObjects)
	{
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
		objects3d->Uninit();
	}
	m_Objects3D.clear();

	// 2dオブジェクトの解放
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->Uninit();
	}

	// システムオブジェクトの解放
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->Uninit();
	}
}

void Scene::UpdateObject()
{
	// グローバルシステムオブジェクトの更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject->Update();
	}

	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->Update();
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->Update();
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
}
void Scene::UpdateObjectByTag(const std::string& tag)
{
	// グローバルなシステムオブジェクトの更新
	for(auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject->Update();
	}

	// システムオブジェクトの更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->UpdateObjectByTag(tag);
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->UpdateObjectByTag(tag);
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
}

void Scene::UpdateObjectByTags(const std::list<std::string>& tags)
{
	// グローバルなシステムオブジェクトの更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject.get()->Update();
	}

	// システムオブジェクトはSystemタグがあれば更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->UpdateObjectByTags(tags);
	}
	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->UpdateObjectByTags(tags);
	}
	DeleteGameObject();
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

	// 2dobj、3dobj用のクラスを作成
	// 2dobjは最後に行くようにする。
	// 3dobjはカメラの位置を考慮し並び替える。
	// ないしは2dと3dobjでそもそも格納する配列を分ける?

	// 3dオブジェクトの描画
	// 描画前にソートし、その後に描画するようにする
	// というか3dobjはそもそもレイヤーいらない説がある(ソートしにくい)

	// 3dオブジェクトの描画
	for(auto& objects3d : m_Objects3D)
	{
		objects3d->Draw();
	}

	// 3dが描画し終わったので2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->Draw();
	}
}

void Scene::DrawObjectByTag(const std::string& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->DrawObjectByTag(tag);
	}

	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->DrawObjectByTag(tag);
	}
}

void Scene::DrawObjectByTags(const std::list<std::string>& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->DrawObjectByTags(tag);
	}
	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->DrawObjectByTags(tag);
	}
}

GameObject* Scene::GetGameObjectByTag(const std::string& tag)
{
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->GetObjectByTag(tag);
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{	
		objects2d->GetObjectByTag(tag);
	}
	return nullptr; // 見つからなかったらnullptrを返す
}

std::list<GameObject*> Scene::GetGameObjectsByTag(const std::string& tag)
{
	std::list<GameObject*> result;
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		objects3d->GetObjectsByTag(tag);
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{
		objects2d->GetObjectsByTag(tag);
	}
	return result; // タグを持つオブジェクトのリストを返す
}

void Scene::UpdateFinal()
{
	// グローバルなシステムオブジェクトの最終更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject.get()->UpdateFinal();
	}

	// systemオブジェクトの最終更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->UpdateFinal();
	}
}

//// 再帰的にGameObjectを探索し、最初に見つかったものを返す
//GameObject* FindGameObjectByTagRecursive(GameObject* obj, const std::string& tag)
//{
//	if (!obj) return nullptr;
//	if (obj->IsTagAvailable(tag)) {
//		return obj;
//	}
//	// Panel型なら子オブジェクトも探索
//	if (auto panel = dynamic_cast<Panel*>(obj)) {
//		for (auto& layer : panel->GetAllChildObjects()) 
//		{
//			for (auto& child : layer)
//			{
//				if (auto found = FindGameObjectByTagRecursive(child.get(), tag))
//				{
//					return found;
//				}
//			}
//		}
//	}
//	return nullptr;
//}
//
//// 再帰的にGameObjectを探索するヘルパー関数
//void FindGameObjectsByTagRecursive(GameObject* obj, const std::string& tag, std::list<GameObject*>& result)
//{
//	if (!obj) return;
//	if (obj->IsTagAvailable(tag)) {
//		result.push_back(obj);
//	}
//	// Panel型なら子オブジェクトも探索
//	if (auto panel = dynamic_cast<Panel*>(obj)) {
//		for (auto& layer : panel->GetAllChildObjects()) 
//		{
//			for (auto& child : layer)
//			{
//				FindGameObjectsByTagRecursive(child.get(), tag, result);
//			}
//		}
//	}
//}