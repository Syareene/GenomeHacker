#include "main.h"
#include "scene/state/setting_state.h"
#include "scene/manager.h"

void SettingState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);
	State::Init();
}

void SettingState::Uninit()
{
	// override後、個別で追加したいものの追加
	State::Uninit();
}

void SettingState::Update()
{
	State::Update();
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("setting");

	UpdateStateObject();
}

void SettingState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTags(std::list<std::string>({ "in_game", "system" }));
	DrawStateObject();

	State::UpdateFinal();
}