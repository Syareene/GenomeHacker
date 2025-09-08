
#include "enemy/dna_screen_script.h"
#include "manager.h"

#include "enemy/node_tab/dnatab_button.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"


// panel型を継承したscript
// 初期化時に属するクラスを勝手に登録する形に
// あとは全体を管理するスクリプトを記載。

void DnaScreenScript::Init(Transform trans)
{
	// 一括管理するために下位オブジェクトを生成

	// これ、パネルの場合表示順いじれないの問題かも?->パネル内の描画は一旦追加順で対処。全体に関してはそもそもベースが描画順コントロールできるからそこでやってくれって感じで(unityも同じだから)
	Panel::AddChildObject(std::make_unique<DNAButton>());
	// 下3つはタブのボタン+タブ内部のスクリプトの描画を管理
	// panelに足すだけじゃなく、すぐ管理できるようにポインタを自身で保持しておく。
	// 生存管理はpanel側で行うので開放処理は必要ない(unique_ptrだしね)
	m_AttackTab = dynamic_cast<AttackTab*>(Panel::AddChildObject(std::make_unique<AttackTab>()));
	m_MoveTab = dynamic_cast<MoveTab*>(Panel::AddChildObject(std::make_unique<MoveTab>()));
	m_DeathTab = dynamic_cast<DeathTab*>(Panel::AddChildObject(std::make_unique<DeathTab>()));


	// 下位オブジェクトをPanelのInitを呼び出し初期化
	Panel::Init();
}

void DnaScreenScript::Uninit()
{
	// DNAスクリーンの終了処理
	Panel::Uninit();
	// ここで必要な終了処理を追加

	// 参照元のオブジェクトは自動開放されるが参照しているポインタにアクセスしようとするとエラーになるためリスト内部をクリアする
	m_AttackTab = nullptr;
	m_MoveTab = nullptr;
	m_DeathTab = nullptr;
}

void DnaScreenScript::Update()
{
	// DNAスクリーンの更新処理
	Panel::Update();
	// ここで必要な更新処理を追加
}

void DnaScreenScript::Draw()
{
	// DNAスクリーンの描画処理
	Panel::Draw();
	// ここで必要な描画処理を追加
}

// enemyのscriptからそれぞれのノードの処理を実行させる関数をここに追加する必要あり


void DnaScreenScript::ShowDnaInfo()
{
	// DNA情報を表示する処理
	// ここで必要な処理を追加
}

void DnaScreenScript::HideDnaInfo()
{

}