#include "scene/game_scene.h"
#include "object/camera.h"
#include "object/field.h"
#include "enemy.h"
#include "player.h"
#include "object/polygon.h"
#include "object/ui/image.h"
#include "vector3.h"

void GameScene::Init()
{
	// ゲームシーンの初期化処理
	AddGameObject<Camera>(0);
	AddGameObject<Field>(0);
	AddGameObject<Enemy>(0)->SetPosition({ 3.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 5.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 8.0f, 0.0f, 0.0f });
	AddGameObject<Player>(0);


	//AddGameObject<Polygon2D>(2);

}

void GameScene::Update()
{
	// ゲームシーンの更新処理
	
	Scene::Update();

	// 
}