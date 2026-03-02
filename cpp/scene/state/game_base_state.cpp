#include "main.h"
#include "scene/state/game_base_state.h"
#include "scene/manager.h"
#include "scene/base_scene.h"

void GameBaseState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);
	State::Init();
	// sceneの描画タグリセット
	Manager::GetCurrentScene()->GetDrawTargetTags().clear();
	// sceneの描画タグセット(このstateではセットしない)
	//Manager::GetCurrentScene()->AddDrawTargetTag();
	// stateの描画タグリセット
	GetDrawTargetTags().clear();
	// stateの描画タグセット(このstateはない)
	//AddDrawTargetTag();
}

void GameBaseState::Uninit()
{
	// override後、個別で追加したいものの追加
	State::Uninit();
}

void GameBaseState::Update()
{
	State::Update();
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObject();

	// このstateのobjectの更新
	UpdateStateObject();
}

void GameBaseState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->DrawObject();
	// このstate内オブジェクトの描画
	DrawStateObject();

	State::UpdateFinal();
}