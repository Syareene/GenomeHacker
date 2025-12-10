#include "main.h"
#include <thread>
#include "scene/state/load_state.h"
#include "scene/manager.h"


void LoadState::Load()
{
	// 先に読み込みたいものの処理を各クラスにload関数として実装
}



void LoadState::Init()
{
	// ロード画面用スクリプトを登録




	// スレッド作成
	std::thread th([=] 
	{
		// load呼び出し

		// 呼び出し後load完了フラグをtrueに
		m_IsLoadComplete = true;
	}); // 一旦仮生成、本来はこのラムダ式内にてstaticのload関数を呼び出す
	th.detach(); // デタッチ(メインスレッドから切り離し非同期処理化)

	
	// 各クラスで定義したあらかじめ読み込みたいものに対しload関数を呼び出す


	// これ以外にもvsのプロパティ変更したので#pragma omp parallel forで並列化できるかも


	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void LoadState::Uninit()
{
	// override後、個別で追加したいものの追加
}

void LoadState::Update()
{
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("setting");

	//UpdateStateObjectByTag("setting");
	UpdateStateObject();


	// load完了していたらstate change
}

void LoadState::Draw()
{
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTags(std::list<std::string>({ "in_game", "system" }));
	// このstate内オブジェクトの描画
	//DrawStateObjectByTag("setting");
	DrawStateObject();

	State::UpdateFinal();
}