#include "main.h"
#include "enemy/node_tab/dnatab_button.h"
#include "scene/manager.h"
#include "scene/game_scene.h"
#include "scene/state/dna_table_state.h"
#include "scene/state/game_base_state.h"

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
	// Use type-based state check
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

	scenePtr->SetState<DnaTableState>(); // changed to template SetState
}