#include "main.h"
#include "scene/title_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "lib/input.h"
#include "scene/game_scene.h"
#include "object/ui/image.h"
#include "object/field.h"
#include "object/camera.h"
#include "collider/collision.h"
#include "object/ui/button.h"
#include "lib/2d_particle.h"

#include "object/ui/font.h"
#include "lib/write_font.h"

#include <memory>

void TitleScene::Init()
{
	// タイトルシーンの初期化処理

	Camera* cam = AddGameObject<Camera>(0);
	cam->SetPosition(Vector3(0.0f, 3.0f, -5.0f));
	cam->SetTitleCam();
	AddGameObject<Field>(1);
	// タイトル
	AddGameObject<ImageDraw>(3)->Register(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 8, 0.0f), Vector3(1024.0f, 576.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\genome_hacker.png", false);

	AddGameObject<Button>(2)->Register([]() {
		// ボタンがクリックされた時の処理
		Manager::SetScene<GameScene>();
		}, Vector2(SCREEN_WIDTH / 2, 600.0f), Vector2(500.0f, 140.0f), 
			Vector2(0.0f, 0.0f), L"asset\\texture\\start_button.png");

	AddGameObject<Particle2D>(1)->Register(1000);
}

void TitleScene::Update()
{
	Scene::UpdateObject();

	// タイトルシーンの更新処理
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらゲームシーンに遷移
		Manager::SetScene<GameScene>();
	}
}

void TitleScene::Draw()
{
	Scene::DrawObject();

	Scene::UpdateFinal();
}