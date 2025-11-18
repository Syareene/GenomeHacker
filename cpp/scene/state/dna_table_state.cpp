#include "main.h"
#include "scene/state/dna_table_state.h"
#include "scene/manager.h"

void DnaTableState::Init()
{
	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void DnaTableState::Uninit()
{
	// override後、個別で追加したいものの追加
}

void DnaTableState::Update()
{
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("dna");
	// このstate内オブジェクトの更新
	UpdateStateObjectByTag("dna");

	// このstateのobjectの更新(これここで基底の変数にアクセスできないから基底に対して更新処理する関数を作成する必要あり)
}

void DnaTableState::Draw()
{
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTag("dna");
	// このstate内オブジェクトの描画
	DrawStateObjectByTag("dna");
}