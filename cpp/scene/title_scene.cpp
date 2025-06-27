#include "scene/title_scene.h"
#include "renderer.h"
#include "manager.h"
#include "object/polygon.h"
#include "input.h"
#include "scene/game_scene.h"

#include <memory>

void TitleScene::Init()
{
	// タイトルシーンの初期化処理
	AddGameObject<Polygon2D>(2); // タイトル画面のポリゴンを追加
}

void TitleScene::Update()
{
	Scene::Update();

	// タイトルシーンの更新処理
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらゲームシーンに遷移
		Manager::SetScene(std::make_unique<GameScene>());
	}
}