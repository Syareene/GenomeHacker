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
#include "enemy/enemy_bullet.h"

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

	Vector3 camRot = AddGameObject<Camera>(0)->GetRotation();
	AddGameObject<Field>(0);
	Transform playerTrans;
	// プレイヤーの向きをカメラの向きに合わせる
	playerTrans.SetRotation(camRot);
	// 生成した角度を元にプレイヤーを生成
	Player* player_ptr = AddGameObject<Player>(0, playerTrans);
	const unsigned int id = player_ptr->GetObjectID();

	AddGameObject<Score>(1);
	AddGameObject<DNAButton>(2);
	AddGameObject<ToGameButton>(2);
	AddSystemObject<EnemyList>(false, id);
	AddSystemObject<EnemySpawner>(false);

	// 敵の弾を予約
	ReserveObject<EnemyBullet>(EnemyBullet::MAX_OBJECTS);

	AddGameObject<Fade>(3)->SetFadeProperty(120, Fade::FadePower::Linear, Fade::FadeTiming::In, true, true); // フェードイン開始

	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\bgm\\EpicBattle_Deity.wav");
	m_BGM->GetSourceVoice()->SetVolume(0.1f); // 音量を設定
	m_BGM->GetSourceVoice()->SetFrequencyRatio(1.0f); // 再生速度を設定
	m_BGM->Play(true);
}

void GameScene::Uninit()
{
	GetCurrentState()->Uninit();
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
	GetCurrentState()->Update();

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
	GetCurrentState()->Draw();

	Scene::UpdateFinal();
	GetCurrentState()->UpdateFinal();

	// stateの移行を実行
	MoveState();
}