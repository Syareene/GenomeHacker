#include "main.h"
#include "enemy/node_tab/dnatab_button.h"
#include "scene/manager.h"
#include "scene/game_scene.h"
#include "scene/state/dna_table_state.h"
#include "scene/state/game_base_state.h"

void DNAButton::Init(Transform trans)
{
	Button::Init(trans);
	FontData fontData;
	fontData.fontSize = 40;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightSteelBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 4.0f;

	// 一旦固定値でセット
	Button::Register(std::bind(&DNAButton::ToDNATab, this), BUTTON_POS,
		BUTTON_SIZE, Vector2(0.0f, 0.0f), fontData, "DNA編集",
		L"asset\\texture\\alpha_texture.png");
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
	// stateを確認
	if (!scenePtr->IsState<GameBaseState>())
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
	// 該当タブの可視性を有効にし、game_sceneのstateをDNAタブに変更
	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scenePtr = dynamic_cast<GameScene*>(gameScene);
	if (scenePtr == nullptr)
	{
		// game_sceneではないのでreturn
		return;
	}

	scenePtr->SetState<DnaTableState>();
}