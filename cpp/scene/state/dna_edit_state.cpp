#include "main.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"

void DnaEditState::Init()
{
	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void DnaEditState::Uninit()
{
	// override後、個別で追加したいものの追加
}

void DnaEditState::Update()
{
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
	// state内オブジェクトの更新
	UpdateStateObjectByTag("dna_edit");
}

void DnaEditState::Draw()
{
	// updateと同じ
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
}