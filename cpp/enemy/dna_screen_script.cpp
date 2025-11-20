#include "main.h"

#include "enemy/dna_screen_script.h"
#include "scene/manager.h"
#include "object/ui/font.h"

#include "enemy/node_tab/dnatab_button.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"


// panel型を継承したscript
// 初期化時に属するクラスを勝手に登録する形に
// あとは全体を管理するスクリプトを記載。

// ここのinit、ゲーム開始時の実行と、stateでの初期化とあるので
// それぞれ処理分けてもいい説はあります

void DnaScreenScript::Init(Transform trans)
{
	SetTransform(trans);
	AddTag("dna_edit");
	
	// 一括管理するために下位オブジェクトを生成

	// これ、パネルの場合表示順いじれないの問題かも?->パネル内の描画は一旦追加順で対処。全体に関してはそもそもベースが描画順コントロールできるからそこでやってくれって感じで(unityも同じだから)
	Panel::AddChildObject<DNAButton>();
	// 下3つはタブのボタン+タブ内部のスクリプトの描画を管理
	// panelに足すだけじゃなく、すぐ管理できるようにポインタを自身で保持しておく。
	// 生存管理はpanel側で行うので開放処理は必要ない(unique_ptrだしね)
	m_AttackTab = Panel::AddChildObject<AttackTab>();
	m_MoveTab = Panel::AddChildObject<MoveTab>();
	m_DeathTab = Panel::AddChildObject<DeathTab>();

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
	if (!IsActive())
	{
		return;
	}
	// DNAスクリーンの更新処理
	Panel::Update();
	// ここで必要な更新処理を追加
}

void DnaScreenScript::Draw()
{
	if(!IsActive())
	{
		return;
	}

	// DNAスクリーンの描画処理
	Panel::Draw();
	// ここで必要な描画処理を追加
}

// enemyのscriptからそれぞれのノードの処理を実行させる関数をここに追加する必要あり


void DnaScreenScript::ShowDnaInfo()
{
	// 表示中かどうかもこっちで管理
	SetActive(true);

	// DNA情報を表示する処理
	// ここで必要な処理を追加

	FontData fontData;
	fontData.fontSize = 120;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 12.0f;

	Panel::AddChildObject<Font>()->Register(Vector2(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6), fontData, "情報表示中");
}

void DnaScreenScript::HideDnaInfo()
{
	// 上でやってるように逆にここはゲームオブジェクトから登録解除し、別で管理する
	// いらないデータは消す
	SetActive(false);

	// panelからfontオブジェクトを消す
	Panel::GetChildObjectByType<Font>()->Destroy();

	// uninitとりあえず呼ぶ
	//Uninit();
}