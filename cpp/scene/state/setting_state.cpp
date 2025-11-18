#include "main.h"
#include "scene/state/setting_state.h"
#include "scene/manager.h"

void SettingState::Init()
{
	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void SettingState::Uninit()
{
	// override後、個別で追加したいものの追加
}

void SettingState::Update()
{
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("setting");

	// このstateのobjectの更新(これここで基底の変数にアクセスできないから基底に対して更新処理する関数を作成する必要あり)
	UpdateStateObjectByTag("setting");
}

void SettingState::Draw()
{
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTags(std::list<std::string>({ "in_game", "system" }));
	// このstate内オブジェクトの描画
	DrawStateObjectByTag("setting");
}