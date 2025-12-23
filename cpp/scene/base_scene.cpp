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

std::list<std::unique_ptr<SystemObject>> Scene::m_GlobalSystemObjects;

void Scene::DeleteGameObject()
{
	// 不要になった3dオブジェクトを削除
	for (auto& objects3d : m_Objects3D)
	{
		// 不要になった GameObject を削除
		objects3d.remove_if([](std::unique_ptr<Object3D>& obj)
			{
				if (obj && obj->Destroy())
				{
					return true;
				}
				return false;
			});
	}

	// 不要になった2dオブジェクトを削除
	for (auto& objects2d : m_Objects2D)
	{
		// 不要になった GameObject を削除
		objects2d.remove_if([](std::unique_ptr<Object2D>& obj)
			{
				if (obj && obj->Destroy())
				{
					return true;
				}
				return false;
			});
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
		for (auto& gameObject : objects3d)
		{
			gameObject->Uninit();
			// smartptrに移行したので、deleteは不要
			// delete gameObject;
		}
	}
	m_Objects3D.clear();

	// 2dオブジェクトの解放
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			gameObject->Uninit();
			// smartptrに移行したので、deleteは不要
			// delete gameObject;
		}
	}
	m_Objects2D.clear();

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
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Update();
		}
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Update();
		}
	}
	// 不要なgameobjectの削除準備
	DeleteGameObject();
}
void Scene::UpdateObjectByTag(const std::string& tag)
{
	// グローバルなシステムオブジェクトの更新
	for(auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject.get()->Update();
	}

	// システムオブジェクトはSystemタグがあれば更新
	for (auto& systemObject : m_SystemObjects)
	{
		if(systemObject.get() == nullptr)
		{
			continue;
		}

		// タグチェック
		if(!systemObject.get()->IsTagAvailable("system"))
		{
			continue; // 該当タグが見つからなかった場合はスルー
		}
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (!gameObject.get()->IsTagAvailable(tag))
			{
				continue; // 該当タグが見つからなかった場合はスルー
			}
			gameObject->Update();
		}
	}

	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (!gameObject.get()->IsTagAvailable(tag))
			{
				continue; // 該当タグが見つからなかったらスルー
			}
			gameObject->Update();
		}
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
		if (systemObject.get() == nullptr)
		{
			continue;
		}

		// タグチェック
		if (!systemObject.get()->IsTagAvailable("system"))
		{
			continue; // 該当タグが見つからなかった場合はスルー
		}
		systemObject->Update();
	}

	// 3dオブジェクトの更新
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Update();
					break;
				}
			}
		}
	}
	// 2dオブジェクトの更新
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& tag : tags)
			{
				if (gameObject.get()->IsTagAvailable(tag))
				{
					// 見つかったら実行しこのオブジェクトに対してのこれ以上の探索はしない
					gameObject->Update();
					break;
				}
			}
		}
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
		// 3Dオブジェクトの描画
		for (auto& gameObject : objects3d)
		{
			// nullチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Draw();
		}
	}

	// 3dが描画し終わったので2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		// 2Dオブジェクトの描画
		for (auto& gameObject : objects2d)
		{
			// nullチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			gameObject->Draw();
		}
	}
}

void Scene::DrawObjectByTag(const std::string& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (!gameObject.get()->IsTagAvailable(tag))
			{
				continue; // 該当タグが見つからなかった場合はスルー
			}
			gameObject->Draw();
		}
	}

	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			if (!gameObject.get()->IsTagAvailable(tag))
			{
				continue; // 該当タグが見つからなかったらスルー
			}
			gameObject->Draw();
		}
	}
}

void Scene::DrawObjectByTags(const std::list<std::string>& tag)
{
	// 3dオブジェクトの描画
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& t : tag)
			{
				if (gameObject.get()->IsTagAvailable(t))
				{
					gameObject->Draw();
					break; // 見つかったらこのオブジェクトに対してのこれ以上の探索はしない
				}
			}
		}
	}
	// 2dオブジェクトの描画
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			// nullptrチェック
			if (gameObject.get() == nullptr)
			{
				continue;
			}
			// タグチェック
			for (const auto& t : tag)
			{
				if (gameObject.get()->IsTagAvailable(t))
				{
					gameObject->Draw();
					break; // 見つかったらこのオブジェクトに対してのこれ以上の探索はしない
				}
			}
		}
	}
}

GameObject* Scene::GetGameObjectByTag(const std::string& tag)
{
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			if (auto found = FindGameObjectByTagRecursive(gameObject.get(), tag)) {
				return found;
			}
		}
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			if (auto found = FindGameObjectByTagRecursive(gameObject.get(), tag)) {
				return found;
			}
		}
	}
	return nullptr; // 見つからなかったらnullptrを返す
}

std::list<GameObject*> Scene::GetGameObjectsByTag(const std::string& tag)
{
	std::list<GameObject*> result;
	// 3Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects3d : m_Objects3D)
	{
		for (auto& gameObject : objects3d)
		{
			FindGameObjectsByTagRecursive(gameObject.get(), tag, result);
		}
	}
	// 2Dオブジェクトからタグを持つオブジェクトを探す
	for (auto& objects2d : m_Objects2D)
	{
		for (auto& gameObject : objects2d)
		{
			FindGameObjectsByTagRecursive(gameObject.get(), tag, result);
		}
	}
	return result; // タグを持つオブジェクトのリストを返す
}

void Scene::UpdateFinal()
{
	// グローバルなシステムオブジェクトの最終更新
	for (auto& systemObject : m_GlobalSystemObjects)
	{
		systemObject.get()->Update();
	}

	// systemオブジェクトの最終更新
	for (auto& systemObject : m_SystemObjects)
	{
		systemObject->UpdateFinal();
	}
}

// 再帰的にGameObjectを探索し、最初に見つかったものを返す
GameObject* FindGameObjectByTagRecursive(GameObject* obj, const std::string& tag)
{
	if (!obj) return nullptr;
	if (obj->IsTagAvailable(tag)) {
		return obj;
	}
	// Panel型なら子オブジェクトも探索
	if (auto panel = dynamic_cast<Panel*>(obj)) {
		for (auto& layer : panel->GetAllChildObjects()) 
		{
			for (auto& child : layer)
			{
				if (auto found = FindGameObjectByTagRecursive(child.get(), tag))
				{
					return found;
				}
			}
		}
	}
	return nullptr;
}

// 再帰的にGameObjectを探索するヘルパー関数
void FindGameObjectsByTagRecursive(GameObject* obj, const std::string& tag, std::list<GameObject*>& result)
{
	if (!obj) return;
	if (obj->IsTagAvailable(tag)) {
		result.push_back(obj);
	}
	// Panel型なら子オブジェクトも探索
	if (auto panel = dynamic_cast<Panel*>(obj)) {
		for (auto& layer : panel->GetAllChildObjects()) 
		{
			for (auto& child : layer)
			{
				FindGameObjectsByTagRecursive(child.get(), tag, result);
			}
		}
	}
}