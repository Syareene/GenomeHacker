#include "main.h"
#include "enemy/node_tab/togame_button.h"
#include "scene/manager.h"
#include "scene/game_scene.h"
#include "scene/state/dna_table_state.h"
#include "scene/state/game_base_state.h"

#include <iostream>


void ToGameButton::Init(Transform trans)
{
	Button::Init(trans);

	FontData fontData;
	fontData.fontSize = 50;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::Gold);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 6.0f;

	// 一旦固定値でセット
	Button::Register(std::bind(&ToGameButton::ToGame, this), ToGameButton::BUTTON_POS,
		ToGameButton::BUTTON_SIZE, Vector2(0.0f, 0.0f), fontData, "戻る",
		L"asset\\texture\\alpha_texture.png");

	// このボタンは最初見えてほしくないので非アクティブ
	SetActive(false);
	AddTag("dna");
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
	// Use type based check
	if (!scenePtr->IsState<DnaTableState>())
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
	scenePtr->SetState<GameBaseState>(); // ゲーム画面stateへ
	
}