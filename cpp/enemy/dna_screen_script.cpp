#include "main.h"

#include "enemy/dna_screen_script.h"
#include "scene/manager.h"
#include "object/ui/font.h"
#include "object/ui/button.h"
#include "object/ui/image.h"

#include "enemy/node_tab/dnatab_button.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"
#include "enemy/base_data/enemy_base.h"
#include "player.h"
#include "lib/input.h" 



// panel型を継承したscript
// 初期化時に属するクラスを勝手に登録する形に
// あとは全体を管理するスクリプトを記載。

// ここのinit、ゲーム開始時の実行と、stateでの初期化とあるので
// それぞれ処理分けてもいい説はあります

void DnaScreenScript::Init(EnemyBase* base_enemy, const unsigned int& player_id)
{
	// プレイヤーid保存
	m_PlayerId = player_id;

	// EnemyBaseからTabManagerを取得
	TabManager* manager = base_enemy->GetTabManager();

	// TabManager経由で対象が所持しているノードの見た目の部分を生成する

	// 初期化
	m_AttackVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetAttackTab());
	m_MoveVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetMoveTab());
	m_DeathVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetDeathTab());

	// プレイヤーにidをセットしてあげる
	Player* player = Manager::GetCurrentScene()->GetGameObject<Player>();
	player->SetDnaScreenId(GetObjectID());

	// これら見た目関数実行した時に元ノードにフラグonにしてこれ以上無駄なノードの生成を防ぐとかはありっす
	// でもそれできるのplayerだけだね

	// プレイヤーに関しても所持しているノードの見た目部分を生成する
	int counter = 0;
	for(auto& node : player->GetAllNodes())
	{
		// nodeの見た目部分を生成
		VisualBase visual = VisualBase();
		visual.Init(GetObjectID(), counter, node.get());

		player->AddVisualNode(visual);
		counter++;
	}


	// 各種ノード
	m_AttackVisual.CreateVisual(manager->GetAttackTab());
	m_MoveVisual.CreateVisual(manager->GetMoveTab());
	m_DeathVisual.CreateVisual(manager->GetDeathTab());

	// その他UI等の生成

	AddTag("dna_edit");

	// 一括管理するために下位オブジェクトを生成

	// これ、パネルの場合表示順いじれないの問題かも?->パネル内の描画は一旦追加順で対処。全体に関してはそもそもベースが描画順コントロールできるからそこでやってくれって感じで(unityも同じだから)
	AddChildObject<DNAButton>(0);

	// 下位オブジェクトをPanelのInitを呼び出し初期化
	Panel::Init();

	// デバッグ用にmoveで表示
	m_MoveVisual.SetIsSelected(true); // 最初は移動タブが選択されている状態にする
}

void DnaScreenScript::Uninit()
{
	// TODO: 残り動いた分の反映処理

	// DNAスクリーンの終了処理
	Panel::Uninit();
	// ここで必要な終了処理を追加

	// 最終的に動いた分を反映
	GetActiveTab()->ApplyMovedResult();


	// playerで保存しているidのリセット
	Manager::GetCurrentScene()->GetGameObject<Player>()->SetDnaScreenId(0);


	// uninit呼び出し
	m_AttackVisual.Uninit();
	m_MoveVisual.Uninit();
	m_DeathVisual.Uninit();

	SetDestroy(true);
}

void DnaScreenScript::Update()
{
	// 有効時の処理
	if (IsActive())
	{
		// DNAスクリーンの更新処理
		//Panel::Update();->こっちで管理したいのでこの下に自作


	// Debug時限定で数字キーでタブ切り替え

	// 1キー: 攻撃タブ
		if (Input::GetKeyTrigger('1'))
		{
			SelectedAttackTab();
		}
		// 2キー: 移動タブ
		if (Input::GetKeyTrigger('2'))
		{
			SelectedMoveTab();
		}
		// 3キー: 死亡タブ
		if (Input::GetKeyTrigger('3'))
		{
			SelectedDeathTab();
		}

		// 子オブジェクトの更新
		for (auto& child : GetAllChildObjects())
		{
			if (!child)
			{
				continue;
			}
			// 更新
			child->Update();
		}

		// パネルの更新処理
		Object2D::Update();
	}

	// 有効でも無効でも下記処理は行う

	// 不要な子オブジェクトの削除処理(最後に呼ぶ)
	DeleteChildObject();

	// 待機オブジェクトの反映
	FlushPendingObjects();
}

void DnaScreenScript::Draw()
{
	if(!IsActive())
	{
		return;
	}

	// 子オブジェクトの描画
	for (auto& child : GetAllChildObjects())
	{
		if (!child)
		{
			continue;
		}
		child->Draw();
	}

	// DNAスクリーンの描画処理
	//Panel::Draw();
	// ここで必要な描画処理を追加
}


void DnaScreenScript::ShowDnaInfo()
{
	// 表示中かどうかもこっちで管理
	SetActive(true);

	// DNA情報を表示する処理

	// 各タブに遷移するテキスト入りボタンを生成
	FontData fontData;
	fontData.fontSize = 35;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::ForestGreen);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 4.0f;


	AddChildObject<Button>(1)->Register([this]() {
		// ボタンがクリックされた時の処理
		SelectedAttackTab();
		}, Vector2(1000.0f, 35.0f), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "攻撃", L"asset\\texture\\alpha_texture.png", L"");
	AddChildObject<Button>(1)->Register([this]() {
		// ボタンがクリックされた時の処理
		SelectedMoveTab();
		}, Vector2(1100.0f, 35.0f), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "移動", L"asset\\texture\\alpha_texture.png", L"");
	AddChildObject<Button>(1)->Register([this]() {
		// ボタンがクリックされた時の処理
		SelectedDeathTab();
		}, Vector2(1200.0f, 35.0f), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "死亡", L"asset\\texture\\alpha_texture.png", L"");

	// 右側の追加したいノード郡
	//Panel::AddChildObject<ImageDraw>(1)->Register(Vector3(950.0f, 50.0f, 0.0f), Vector3(400.0f, 70.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\debug_sprite.png");

	// 表示されたりされなかったりするなこれ->消してないのもあるし位置調整含めて後々でいいか
	// 描画されてない時、game_objのリストにはあるが範囲forにてヒットしておらず描画されない?
	AddChildObject<ImageDraw>(1)->Register(Vector3(1024.0f, 450.0f, 0.0f), Vector3(512.0f, 540.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\node_list.png");


	// 現在のノードを表示
	fontData.fontSize = 100;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::LightBlue);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 12.0f;


	// 選択されているタブに応じてフォントを生成
	if(m_AttackVisual.GetIsSelected())
	{
		AddChildObject<Font>(0)->Register(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT / 8), fontData, "攻撃ノード");
		return;
	}
	if(m_MoveVisual.GetIsSelected())
	{
		AddChildObject<Font>(0)->Register(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT / 8), fontData, "移動ノード");
		return;
	}
	if(m_DeathVisual.GetIsSelected())
	{
		AddChildObject<Font>(0)->Register(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT / 8), fontData, "死亡ノード");
		return;
	}
}

void DnaScreenScript::HideDnaInfo()
{
	// 上でやってるように逆にここはゲームオブジェクトから登録解除し、別で管理する
	// いらないデータは消す
	SetActive(false);

	// ここTabBase以外を消すようにしたほうがいいな


	// ここでちゃんと子オブジェクト消せてなさそうね

	// panelからfontオブジェクトを消す
	for(auto& child : GetChildObjectsByType<Font>())
	{
		child.SetDestroy(true);
	}

	// buttonも消す
	// これでもDNAButtonとかも消えちゃうからタグつけないとだ
	for(auto& child : GetChildObjectsByType<Button>())
	{
		child.SetDestroy(true);
	}

	// 明示的に削除する(次fのupdateでDestroyが呼ばれないため)->一時的処理であるかも
	DeleteChildObject();

	// uninitとりあえず呼ぶ
	//Uninit();
}

TabVisual* DnaScreenScript::GetActiveTab()
{
	if(m_AttackVisual.GetIsSelected())
	{
		return &m_AttackVisual;
	}
	if(m_MoveVisual.GetIsSelected())
	{
		return &m_MoveVisual;
	}
	if(m_DeathVisual.GetIsSelected())
	{
		return &m_DeathVisual;
	}
	return nullptr;
}

void DnaScreenScript::SelectedAttackTab()
{
	GetChildObjectByType<Font>()->SetDisplayText("攻撃ノード");
	m_AttackVisual.SetIsSelected(true);
	m_AttackVisual.ModifyNodePos(); // ノード位置修正
	m_MoveVisual.SetIsSelected(false);
	m_DeathVisual.SetIsSelected(false);
}

void DnaScreenScript::SelectedMoveTab()
{
	GetChildObjectByType<Font>()->SetDisplayText("移動ノード");
	m_AttackVisual.SetIsSelected(false);
	m_MoveVisual.SetIsSelected(true);
	m_MoveVisual.ModifyNodePos(); // ノード位置修正
	m_DeathVisual.SetIsSelected(false);
}

void DnaScreenScript::SelectedDeathTab()
{
	GetChildObjectByType<Font>()->SetDisplayText("死亡ノード");
	m_AttackVisual.SetIsSelected(false);
	m_MoveVisual.SetIsSelected(false);
	m_DeathVisual.SetIsSelected(true);
	m_DeathVisual.ModifyNodePos(); // ノード位置修正
}