#include "scene/base_scene.h"
#include "renderer.h"
#include "manager.h"
#include "object/camera.h"

void Scene::Init()
{
	/*
	AddGameObject<Camera>(0);
	AddGameObject<Field>(0);
	AddGameObject<Enemy>(0)->SetPosition({ 3.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 5.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 8.0f, 0.0f, 0.0f });
	AddGameObject<Player>(0);


	AddGameObject<Polygon2D>(2);
	*/
}


void Scene::Uninit()
{
	for (auto& gameObjects : m_GameObjects)
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->Uninit();
			delete gameObject;
		}
	}
	m_GameObjects.clear();
}

void Scene::Update()
{
	for (auto& gameObjects : m_GameObjects)
	{
		for (auto& gameObject : gameObjects)
		{
			// nullptrチェック
			if (gameObject == nullptr)
			{
				continue;
			}
			gameObject->Update();
		}
	}


	for (auto& gameObjects : m_GameObjects)
	{
		// 不要になった GameObject を削除
		gameObjects.remove_if([](GameObject* obj)
			{
				if (obj && obj->Destory())
				{
					return true;
				}
				return false;
			});
	}
}

void Scene::Draw()
{
	// 描画順を揃えるためにソートする(listのアルゴリズム使用)->3dobjだけでいいからそこは工夫したいね
	// 本当はカメラのz方向の距離でソートしないとダメ
	// m_GameObject[1].sort([&](GameObject* a, GameObject* b)
	//{
		// ここで描画順を決める
		// 例えば、Z座標が小さい方を先に描画するなど
	//	return a->GetDistance(cameraPosition) > b->GetDistance(cameraPosition);
	//});

	for (auto& gameObjects : m_GameObjects)
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->Draw();
		}
	}
}