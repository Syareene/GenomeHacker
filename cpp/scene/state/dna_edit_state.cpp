#include "main.h"
#include "scene/state/dna_edit_state.h"
#include "scene/state/dna_table_state.h"
#include "scene/manager.h"
#include "object/ui/button.h"
#include "manager/texture_manager.h"
#include "enemy/base_data/enemy_base.h"

#include "enemy/node/base.h"

void DnaEditState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);

	State::Init();

	AddGameObject<Button>(0)->Register([this]() {
		// ボタンがクリックされた時の処理
			Clicked();
			}, Vector2(125.0f, 40.0f), Vector2(250.0f, 80.0f), Vector2(0.0f, 0.0f), TextureManager::LoadTexture(L"asset/texture/return_temp.png"));

	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void DnaEditState::Uninit()
{
	// override後、個別で追加したいものの追加
	State::Uninit();
}

void DnaEditState::Update()
{
	State::Update();
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");

	m_CurrentEnemyBase->GetDnaScreen()->Update();

	// state内オブジェクトの更新
	//UpdateStateObjectByTag("dna_edit");
	UpdateStateObject();
	DeleteGameObject();
}

void DnaEditState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
	m_CurrentEnemyBase->GetDnaScreen()->Draw();
	// state内オブジェクトの描画
	DrawStateObject();

	State::UpdateFinal();
}

void DnaEditState::Clicked()
{
	// state変更
	Manager::GetCurrentScene()->SetState<DnaTableState>();
}