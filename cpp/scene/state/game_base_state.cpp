#include "main.h"
#include "scene/state/game_base_state.h"
#include "scene/manager.h"

void GameBaseState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);
	State::Init();
	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
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

	// このstateのobjectの更新(これここで基底の変数にアクセスできないから基底に対して更新処理する関数を作成する必要あり)
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