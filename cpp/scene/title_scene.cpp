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

	// タイトル背景
	//AddGameObject<ImageDraw>(0)->FirstInit(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\debug_sprite.png", false);
	AddGameObject<Camera>(0)->SetPosition(Vector3(0.0f, 3.0f, -5.0f));
	AddGameObject<Field>(1);
	// タイトル
	AddGameObject<ImageDraw>(3)->FirstInit(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 8, 0.0f), Vector3(1024.0f, 576.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\genome_hacker.png", false);

	AddGameObject<Button>(2)->Register([]() {
		// ボタンがクリックされた時の処理
		Manager::SetScene<GameScene>();
		}, Vector2(SCREEN_WIDTH / 2, 600.0f), Vector2(500.0f, 140.0f), 
			Vector2(0.0f, 0.0f), L"asset\\texture\\start_button.png");
	FontData fontData;
	fontData.fontSize = 80;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::Crimson);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 8.0f;

	//AddGameObject<Font>(3)->Register(Vector2(200.0f, 275.0f), fontData, "サブタイトルをフォントで描画");

	fontData.fontSize = 60;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightGoldenrodYellow);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 7.0f;

	//AddGameObject<Font>(3)->Register(Vector2(600.0f, 400.0f), fontData, "縁取りも色も変更可能");
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