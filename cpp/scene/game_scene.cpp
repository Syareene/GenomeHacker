#include "scene/game_scene.h"
#include "object/camera.h"
#include "object/field.h"
#include "enemy.h"
#include "player.h"
#include "object/polygon.h"
#include "object/ui/image.h"
#include "vector3.h"
#include "particle.h"
#include "input.h"
#include "result_scene.h"
#include "manager.h"
#include "score.h"
#include "lib/audio.h"

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
	AddGameObject<Score>(0);


	//AddGameObject<Polygon2D>(2);
	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\bgm\\cafe_seaside.wav");
	m_BGM->GetSourceVoice()->SetVolume(0.2f); // 音量を設定
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