#include "main.h"
#include "scene/game_scene.h"
#include "object/camera.h"
#include "object/field.h"
#include "player.h"
#include "object/ui/image.h"

#include "particle.h"
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
#include "enemy/node_tab/enemy_dna_list.h"

void GameScene::Init()
{
	// ゲームシーンの初期化処理

	AddGameObject<Camera>(0);
	AddGameObject<Field>(0);
	AddGameObject<Player>(0);
	//AddGameObject<Particle>(0)->SetPosition({ 0.0f, 3.0f, 0.0f });
	AddGameObject<Score>(1);
	AddGameObject<DNAButton>(2);
	AddGameObject<ToGameButton>(2);
	AddSystemObject<EnemyList>();
	AddSystemObject<EnemyDnaList>();
	AddSystemObject<EnemySpawner>();
	//AddGameObject<ImageDraw>(2)->FirstInit(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), Vector3(1000.0f, 200.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\temp_title.png", false);
	

	// 撮影用に一旦見せなくする
	/*
	AddGameObject<Button>(2)->Register([this]() {
		// ボタンがクリックされた時の処理
		GetGameObject<Score>()->AddScore(10);
	}, Vector2(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 350.0f), Vector2(300.0f, 300.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png", L"asset\\texture\\test_frame.png");

	AddGameObject<Button>(1)->Register([]() {
		// ボタンがクリックされた時の処理
		Manager::SetScene(std::make_unique<ResultScene>());
		}, Vector2(SCREEN_WIDTH - 150.0f, 50.0f), Vector2(300.0f, 100.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png");
	*/


	/*
	AddGameObject<Button>(2)->Register([this]() {
		// ボタンがクリックされた時の処理
		dynamic_cast<Button*>(GetGameObjectByTag("aiueo"))->GetDistance(Vector3(0.0f, 0.0f, 0.0f));
		}, Vector2(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - 200.0f), Vector2(300.0f, 100.0f), Vector2(0.0f, 0.0f), L"asset\\texture\\bomb.png");
		*/

	//AddGameObject<Polygon2D>(2);
	AddGameObject<Fade>(3)->SetFadeProperty(120, Fade::FadePower::Linear, Fade::FadeTiming::In, true, true); // フェードイン開始

	m_BGM = new Audio();
	m_BGM->Load("asset\\audio\\bgm\\EpicBattle_Deity.wav");
	m_BGM->GetSourceVoice()->SetVolume(0.1f); // 音量を設定
	m_BGM->GetSourceVoice()->SetFrequencyRatio(1.0f); // 再生速度を設定
	m_BGM->Play(true);

	m_State = State::NORMAL; // 初期状態をNORMALに設定
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
	// 本来ならScene::Update()を呼べばいいが、stateによる管理をしたいので、、って感じ
	// どのみち判別方法がタブならSceneの方に特定のタグが付いているobjectかどうかってのを比較してからupdateすれば、、、

	// このシーンにおいてはscene内でのstateの情報と、すべてのgameobjのリストとは別に現在のstateで更新するオブジェクトのポインタを保持する配列が必要。
	// その配列を使用しオブジェクトを更新する。もしstateの更新が入った場合はオブジェクトのポインタを保持する配列を、すべてのgameobjのリストから現在のstateに該当するものだけを抽出し、更新する。
	// AddGameobj関数をoverrideし、追加する際にタグ等から現在のstateに対して追加してもいいかどうかを関数内で判断する(引数は変更できないから)処理を追加する必要あり。
	// gameobjのタグに対してfieldobjとかsettingsobjとかのタグをもたせる感じかな、、、　更新はしないけど描画はするみたいな設定も欲しくはある。

	// ゲーム時に使うobjectはInGameタグを付ける
	// んで多分escメニューのときは更新切って描画だけ、dnaタブのときはどっちも切るみたいなことをするからそのときにどうするかみたいな話ではある。

	switch (m_State)
	{
		// 特定のタグだけに対して更新処理を実行する関数を作ってもいいんだが、単一タグだけじゃない可能性があるのが
		// 両方の関数作ればよいか、、

		// 通常時処理
		case State::NORMAL:
			Scene::Update();
			break;
		// escメニュー出したとき(更新はせずobjの描画はする)
		case State::ESC_MENU:
			// Sceneのupdateとは別でシステム系のやつだけ更新をいれる
			Scene::UpdateObjectByTag("System");
			break;
		// dnaタブのときやその他ウィンドウ系(更新も描画もしない)
		case State::DNA_TAB:
			// Sceneのupdateとは別で今このシーンで使うやつにのみupdateを入れる
			Scene::UpdateObjectByTag("Dna");
			break;
		default:
			break;
	}
	

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		// Enterキーが押されたらリザルトシーンに遷移
		Manager::SetScene<ResultScene>();
	}
}

void GameScene::Draw()
{
	switch (m_State)
	{
		// 通常時処理
	case State::NORMAL:
		Scene::Draw();
		break;
		// escメニュー出したとき(更新はせずobjの描画はする)
	case State::ESC_MENU:
		Scene::DrawObjectByTags(std::list<std::string>({"InGame", "System"}));
		break;
		// dnaタブのときやその他ウィンドウ系(更新も描画もしない)
	case State::DNA_TAB:
		Scene::DrawObjectByTag("Dna");
		break;
	default:
		break;
	}

	// ゲームシーン内の描画が終わったのでstate変更処理

	// state保存
	m_BeforeState = m_State;
	// state変更があった場合は変更
	if (m_WillState != m_State)
	{
		m_State = m_WillState;
	}
}