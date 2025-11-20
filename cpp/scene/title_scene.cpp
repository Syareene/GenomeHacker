#include "main.h"
#include "scene/title_scene.h"
#include "lib/renderer.h"
#include "scene/manager.h"
#include "lib/input.h"
#include "scene/game_scene.h"
#include "object/ui/image.h"
#include "object/ui/button.h"

#include "object/ui/font.h"

#include "lib/write_font.h"

#include <memory>

void TitleScene::Init()
{
	// タイトルシーンの初期化処理
	
	// タイトル
	AddGameObject<ImageDraw>(2)->FirstInit(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), Vector3(1000.0f, 200.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\temp_title.png", false);

	AddGameObject<Button>(1)->Register([]() {
		// ボタンがクリックされた時の処理
		Manager::SetScene<GameScene>();
		}, Vector2(SCREEN_WIDTH / 2, 600.0f), Vector2(500.0f, 140.0f), 
			Vector2(0.0f, 0.0f), L"asset\\texture\\start_button.png");
	FontData fontData;
	fontData.fontSize = 120;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 12.0f;

	AddGameObject<Font>(3)->Register(Vector2(200.0f, 100.0f), fontData, "タイトル");
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