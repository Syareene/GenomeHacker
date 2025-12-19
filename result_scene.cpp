#include "main.h"
#include "result_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "lib/input.h"
#include "scene/title_scene.h"
#include "object/ui/image.h"

#include <memory>

void ResultScene::Init()
{
	// タイトルシーンの初期化処理

	// タイトル
	AddGameObject<ImageDraw>(2)->Register(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), Vector3(1000.0f, 200.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\temp_result.png", false);
}

void ResultScene::Update()
{
	Scene::UpdateObject();

	// タイトルシーンの更新処理
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらタイトルシーンに遷移
		Manager::SetScene<TitleScene>();
	}
}

void ResultScene::Draw()
{
	Scene::DrawObject();

	Scene::UpdateFinal();

}