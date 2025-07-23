#include "scene/game_scene.h"
#include "object/camera.h"
#include "object/field.h"
#include "enemy/enemy.h"
#include "player.h"
#include "object/polygon.h"
#include "object/ui/image.h"
#include "lib/vector3.h"
#include "particle.h"
#include "input.h"
#include "result_scene.h"
#include "manager.h"
#include "score.h"
#include "lib/audio.h"
#include "object/ui/button.h"

void GameScene::Init()
{
	// ゲームシーンの初期化処理
	AddGameObject<Camera>(0);
	AddGameObject<Field>(0);
	AddGameObject<Enemy>(0)->SetPosition({ 3.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 5.0f, 0.0f, 0.0f });
	AddGameObject<Enemy>(0)->SetPosition({ 8.0f, 0.0f, 0.0f });
	AddGameObject<Player>(0);
	//AddGameObject<Particle>(0)->SetPosition({ 0.0f, 3.0f, 0.0f });
	AddGameObject<Score>(1);
	//AddGameObject<ImageDraw>(2)->FirstInit(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), Vector3(1000.0f, 200.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\temp_title.png", false);
	//AddGameObject<ImageDraw>(2)->FirstInit(Vector3(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 150.0f, 0.0f), Vector3(300.0f, 300.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\bomb.png", false);
	AddGameObject<Button>(2)->Register([this]() {
		// ボタンがクリックされた時の処理
		GetGameObject<Score>()->AddScore(10);
	}, Vector2(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 350.0f), Vector2(300.0f, 300.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png");

	AddGameObject<Button>(2)->Register([]() {
		// ボタンがクリックされた時の処理
		Manager::SetScene(std::make_unique<ResultScene>());
		}, Vector2(SCREEN_WIDTH - 150.0f, 50.0f), Vector2(300.0f, 100.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png");

	/*
	AddGameObject<Button>(2)->Register([this]() {
		// ボタンがクリックされた時の処理
		dynamic_cast<Button*>(GetGameObjectByTag("aiueo"))->GetDistance(Vector3(0.0f, 0.0f, 0.0f));
		}, Vector2(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 200.0f), Vector2(300.0f, 100.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png");
		*/

	//AddGameObject<Polygon2D>(2);
	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\bgm\\cafe_seaside.wav");
	m_BGM->GetSourceVoice()->SetVolume(0.15f); // 音量を設定
	m_BGM->GetSourceVoice()->SetFrequencyRatio(1.0f); // 再生速度を設定
	m_BGM->Play(true);
}

void GameScene::Uninit()
{
	// ゲームシーンの終了処理
	Scene::Uninit();
	// BGMの解放
	if (m_BGM)
	{
		m_BGM->Uninit();
		delete m_BGM;
		m_BGM = nullptr;
	}
}

void GameScene::Update()
{
	// ゲームシーンの更新処理
	
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらリザルトシーンに遷移
		Manager::SetScene(std::make_unique<ResultScene>());
	}

	// 
}