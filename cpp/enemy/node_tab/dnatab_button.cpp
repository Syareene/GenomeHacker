#include "enemy/node_tab/dnatab_button.h"
#include "manager.h"
#include "scene/game_scene.h"

void DNAButton::Init(Transform trans)
{
	Button::Init(trans);
	// 一旦固定値でセット

	Button::Register(std::bind(&DNAButton::ToDNATab, this), Vector2(SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2),
		Vector2(200.0f, 100.0f), Vector2(0.0f, 0.0f),
		L"asset/texture/debug_sprite.png");
}

void DNAButton::Uninit()
{
	Button::Uninit();
}

void DNAButton::Update()
{
	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scenePtr = dynamic_cast<GameScene*>(gameScene);
	if (scenePtr == nullptr)
	{
		// game_sceneではないのでreturn
		return;
	}
	if (scenePtr->GetState() != GameScene::State::NORMAL)
	{
		SetActive(false);
		return;
	}
	SetActive(true);

	Button::Update();
}

void DNAButton::Draw()
{
	if (!IsActive())
	{
		return;
	}
	Button::Draw();
}


void DNAButton::ToDNATab()
{
	// んー、こいつらって実態としてもたせるのかどうかを忘れてしもた
	// 実態としてもたせるならそもそもゲームシーン側のボタンの見た目とdna側での見た目と違うからややこしいような。

	// DNAタブに移動する処理
	
	// 該当タブの可視性を有効にし、game_sceneのstateをtabにする
	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scenePtr = dynamic_cast<GameScene*>(gameScene);
	if (scenePtr == nullptr)
	{
		// game_sceneではないのでreturn
		return;
	}
	SetActive(false); // 自身をアクティブにする

	scenePtr->SetState(GameScene::State::DNA_TAB); // ゲームシーンの状態をDNAタブに変更
	
}