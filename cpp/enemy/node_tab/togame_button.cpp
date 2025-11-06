#include "main.h"
#include "enemy/node_tab/togame_button.h"
#include "scene/manager.h"
#include "scene/game_scene.h"

#include <iostream>


void ToGameButton::Init(Transform trans)
{
	Button::Init(trans);
	// 一旦固定値でセット

	// 登録
	/*
	Button::Register(std::bind(&ToGameButton::ToGame, this), Vector2(trans.GetPosition().x, trans.GetPosition().y), 
		Vector2(trans.GetScale().x, trans.GetScale().y),Vector2(trans.GetRotation().x, trans.GetRotation().y), 
		L"resources/textures/ui/to_game_button.png");
	*/

	Button::Register(std::bind(&ToGameButton::ToGame, this), ToGameButton::BUTTON_POS,
		ToGameButton::BUTTON_SIZE, Vector2(0.0f, 0.0f),
		L"asset/texture/return_temp.png");

	// このボタンは最初見えてほしくないので非アクティブ
	SetActive(false);
	AddTag("Dna");
}

void ToGameButton::Uninit()
{
	Button::Uninit();
}

void ToGameButton::Update()
{
	// 変わったことはこっち側で検知して表示非表示を管理したいな、、

	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scenePtr = dynamic_cast<GameScene*>(gameScene);
	if (scenePtr == nullptr)
	{
		std::cerr << "テストエラー" << std::endl;
		// game_sceneではないのでreturn
		return;
	}
	if (scenePtr->GetState() != GameScene::State::DNA_TAB)
	{
		SetActive(false);
		return;
	}
	SetActive(true);

	Button::Update();
}

void ToGameButton::Draw()
{
	if (!IsActive())
	{
		return;
	}

	Button::Draw();
}

// コールバックに登録したいやつ
void ToGameButton::ToGame()
{
	// 該当タブの可視性を有効にし、game_sceneのstateをtabにする
	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scenePtr = dynamic_cast<GameScene*>(gameScene);
	if (scenePtr == nullptr)
	{
		std::cerr << "テストエラー" << std::endl;
		// game_sceneではないのでreturn
		return;
	}
	scenePtr->SetState(GameScene::State::NORMAL); // ゲームシーンの状態を通常に変更
	
}