#include "main.h"

#include "enemy/dna_screen_script.h"
#include "scene/manager.h"
#include "object/ui/font.h"

#include "enemy/node_tab/dnatab_button.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

#include "lib/input.h" 


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

	//m_AttackTab->SetIsSelected(true); // 最初は攻撃タブが選択されている状態にする

	// デバッグ用にmoveで表示
	m_MoveTab->SetIsSelected(true); // 最初は移動タブが選択されている状態にする
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
	//Panel::Update();->こっちで管理したいのでこの下に自作


	// Debug時限定で数字キーでタブ切り替え
	
	// 1キー: 攻撃タブ
	if(Input::GetKeyTrigger('1'))
	{
		Panel::GetChildObjectByType<Font>()->SetDisplayText("攻撃ノード表示中");
		m_AttackTab->SetIsSelected(true);
		m_MoveTab->SetIsSelected(false);
		m_DeathTab->SetIsSelected(false);
	}
	// 2キー: 移動タブ
	if (Input::GetKeyTrigger('2'))
	{
		Panel::GetChildObjectByType<Font>()->SetDisplayText("移動ノード表示中");
		m_AttackTab->SetIsSelected(false);
		m_MoveTab->SetIsSelected(true);
		m_DeathTab->SetIsSelected(false);
	}
	// 3キー: 死亡タブ
	if(Input::GetKeyTrigger('3'))
	{
		Panel::GetChildObjectByType<Font>()->SetDisplayText("死亡ノード表示中");
		m_AttackTab->SetIsSelected(false);
		m_MoveTab->SetIsSelected(false);
		m_DeathTab->SetIsSelected(true);
	}


	// 子オブジェクトの更新
	for (auto& child : GetChildObjects())
	{
		if (TabBase* temp = static_cast<TabBase*>(child))
		{
			// TabBaseの場合は選択されていないならskip
			if(!temp->GetIsSelected())
			{
				continue;
			}
		}
		child->Update();
	}


	// パネルの更新処理
	Object2D::Update();



	// ここで必要な更新処理を追加




	// 不要な子オブジェクトの削除処理(最後に呼ぶ)
	DeleteChildObject();	
}

void DnaScreenScript::Draw()
{
	if(!IsActive())
	{
		return;
	}

	// 子オブジェクトの更新
	for (auto& child : GetChildObjects())
	{
		if (TabBase* temp = static_cast<TabBase*>(child))
		{
			// TabBaseの場合は選択されていないならskip
			if (!temp->GetIsSelected())
			{
				continue;
			}
		}
		child->Draw();
	}

	// DNAスクリーンの描画処理
	Panel::Draw();
	// ここで必要な描画処理を追加
}


void DnaScreenScript::ShowDnaInfo()
{
	// 表示中かどうかもこっちで管理
	SetActive(true);

	// DNA情報を表示する処理
	// ここで必要な処理を追加

	FontData fontData;
	fontData.fontSize = 120;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 12.0f;

	// 選択されているタブに応じてフォントを生成
	if(m_AttackTab->GetIsSelected())
	{
		Panel::AddChildObject<Font>()->Register(Vector2(0.0f, SCREEN_HEIGHT / 8), fontData, "攻撃ノード表示中");
		return;
	}
	if(m_MoveTab->GetIsSelected())
	{
		Panel::AddChildObject<Font>()->Register(Vector2(0.0f, SCREEN_HEIGHT / 8), fontData, "移動ノード表示中");
		return;
	}
	if(m_DeathTab->GetIsSelected())
	{
		Panel::AddChildObject<Font>()->Register(Vector2(0.0f, SCREEN_HEIGHT / 8), fontData, "死亡ノード表示中");
		return;
	}
}

void DnaScreenScript::HideDnaInfo()
{
	// 上でやってるように逆にここはゲームオブジェクトから登録解除し、別で管理する
	// いらないデータは消す
	SetActive(false);

	// panelからfontオブジェクトを消す
	for(auto& child : GetChildObjectsByType<Font>())
	{
		child->SetDestory(true);
	}

	// 明示的に削除する(次fのupdateでDestroyが呼ばれないため)->一時的処理であるかも
	DeleteChildObject();

	// uninitとりあえず呼ぶ
	//Uninit();
}

TabBase* DnaScreenScript::GetActiveTab()
{
	if (m_AttackTab->GetIsSelected())
	{
		return m_AttackTab;
	}
	if (m_MoveTab->GetIsSelected())
	{
		return m_MoveTab;
	}
	if (m_DeathTab->GetIsSelected())
	{
		return m_DeathTab;
	}
	return nullptr;
}