#include "main.h"
#include "scene/game_scene.h"
#include "object/camera.h"
#include "object/field.h"
#include "player.h"
#include "object/ui/image.h"

#include "lib/particle.h"
#include "lib/input.h"
#include "result_scene.h"
#include "scene/manager.h"
#include "score.h"
#include "lib/audio.h"
#include "object/ui/button.h"
#include "enemy/enemy_spawner.h"
#include "enemy/node_tab/dnatab_button.h"
#include "enemy/node_tab/togame_button.h"
#include "object/ui/in_game/fade.h"
#include "enemy/base_data/enemy_list.h"
//#include "enemy/node_tab/enemy_dna_list.h"

#include <typeindex>
// state系
#include "scene/state/game_base_state.h"
#include "scene/state/dna_table_state.h"
#include "scene/state/dna_edit_state.h"

void GameScene::Init()
{
	// ゲームシーンの初期化処理
	SetState<GameBaseState>()->Init();
	// 明示的にstateをセットして遷移実行
	MoveState();

	AddGameObject<Camera>(0);
	AddGameObject<Field>(0);
	AddGameObject<Player>(0);
	//AddGameObject<Particle>(0)->SetPosition({ 0.0f, 3.0f, 0.0f });
	AddGameObject<Score>(1);
	AddGameObject<DNAButton>(2);
	AddGameObject<ToGameButton>(2);
	AddSystemObject<EnemyList>();
	AddSystemObject<EnemySpawner>();
	
	// ボタン用フォントデータ設定
	FontData fontData;
	fontData.fontSize = 50;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 4.0f;

	
	AddGameObject<Button>(2)->Register([this]() {
		// ボタンがクリックされた時の処理
		GetGameObject<Player>()->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
		}, Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), Vector2(200.0f, 100.0f), 
			Vector2(0.0f, 0.0f), fontData, "ボタン", L"asset\\texture\\debug_sprite.png", L"");

	//AddGameObject<Polygon2D>(2);
	AddGameObject<Fade>(3)->SetFadeProperty(120, Fade::FadePower::Linear, Fade::FadeTiming::In, true, true); // フェードイン開始

	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\bgm\\EpicBattle_Deity.wav");
	m_BGM->GetSourceVoice()->SetVolume(0.1f); // 音量を設定
	m_BGM->GetSourceVoice()->SetFrequencyRatio(1.0f); // 再生速度を設定
	m_BGM->Play(true);
}

void GameScene::Uninit()
{
	GetStatePtr()->Uninit();
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
	// 現在のstateに応じてupdateを実行
	GetStatePtr()->Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらリザルトシーンに遷移
		Manager::SetScene<ResultScene>();
	}

	// フレーム内の更新が終わったら state-changed フラグをクリア
	ResetStateChanged();
}

void GameScene::Draw()
{
	// 現在のstateに応じてdrawを実行
	GetStatePtr()->Draw();

	Scene::UpdateFinal();
	GetStatePtr()->UpdateFinal();

	// stateの移行を実行
	MoveState();
}