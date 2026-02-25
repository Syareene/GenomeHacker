#include "main.h"

#include "enemy/dna_screen_script.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
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

void DnaScreenScript::Init(EnemyBase* base_enemy, const unsigned int& player_id)
{
	m_EnemyBase = base_enemy;

	// プレイヤーid保存
	m_PlayerId = player_id;

	// EnemyBaseからTabManagerを取得
	TabManager* manager = base_enemy->GetTabManager();

	// TabManager経由で対象が所持しているノードの見た目の部分を生成する


	// screenのidちゃんと発行されてない

	// 初期化
	m_AttackVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetAttackTab());
	m_MoveVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetMoveTab());
	m_DeathVisual.Init(GetObjectID(), player_id, base_enemy->GetTabManager()->GetDeathTab());

	// プレイヤーにidをセットしてあげる
	Player* player = Manager::GetCurrentScene()->GetGameObject<Player>();
	player->SetDnaScreenId(GetObjectID());

	// プレイヤーに関しても所持しているノードの見た目部分を生成する
	GeneratePlayerVisualNodes();

	// その他UI等の生成

	AddTag("dna_edit");

	// 下位オブジェクトをPanelのInitを呼び出し初期化
	Panel::Init();

	// デバッグ用にmoveで表示
	m_MoveVisual.SetIsSelected(true); // 最初は移動タブが選択されている状態にする
}

void DnaScreenScript::Uninit()
{
	// ここで必要な終了処理を追加

	// 最終的に動いた分を反映
	GetActiveTab()->ApplyMovedResult();


	// playerで保存しているidのリセット
	Manager::GetCurrentScene()->GetGameObject<Player>()->SetDnaScreenId(0);


	// uninit呼び出し
	m_AttackVisual.Uninit();
	m_MoveVisual.Uninit();
	m_DeathVisual.Uninit();

	m_EnemyBase = nullptr;

	// 最初に解放するのではなく最後に解放するように変更!
	// DNAスクリーンの終了処理
	Panel::Uninit();

	SetDestroy(true);
}

void DnaScreenScript::Update()
{
	// 有効時の処理
	if (IsActive())
	{
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

		// タブ更新(内部でアクティブなタブのみ更新される)
		m_AttackVisual.Update();
		m_MoveVisual.Update();
		m_DeathVisual.Update();

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

		// ノードを離したかどうかのフラグ確認
		if (m_IsReleaseGrabNode)
		{
			// もし掴んでるノードがあるなら挿入処理関数を実行
			if (m_GrabbingNode)
			{
					GetActiveTab()->ApplyGrabNode();
			}
			m_GrabbingNode = nullptr; // 掴んでいるノードを離す
			m_IsReleaseGrabNode = false;
		}
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

	// タブ描画(内部でアクティブなタブのみ描画される)
	m_AttackVisual.Draw();
	m_MoveVisual.Draw();
	m_DeathVisual.Draw();

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
	fontData.Color = D2D1::ColorF(D2D1::ColorF::OrangeRed);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 4.0f;


	// ラムダ式のキャプチャにthisを入れていたが、pendingでmoveする関係上thisだと動かない
	// そのためオブジェクトに設定されているidを用いて再取得する形に変更しボタンのコールバックを設定

	unsigned int myID = GetObjectID();
	// コールバック関数
	auto buttonCallback = [myID](int type) 
	{
		// 現在のシーンからIDを使って自身を再取得
		auto scene = Manager::GetCurrentScene().get();
		if (auto script = scene->GetCurrentState()->GetGameObjectById<DnaScreenScript>(myID))
		{
			// 有効なインスタンスに対して処理を実行
			switch (type) 
			{
				case 0: 
					script->SelectedAttackTab(); 
					break;
				case 1: 
					script->SelectedMoveTab(); 
					break;
				case 2: 
					script->SelectedDeathTab(); 
					break;
			}
		}
	};

	AddChildObject<Button>(1)->Register([buttonCallback]() {
		buttonCallback(0); // 攻撃
		}, Vector2(NODE_TAB_TEXT_POS), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "攻撃", L"asset\\texture\\alpha_texture.png", L"");

	fontData.Color = D2D1::ColorF(D2D1::ColorF::ForestGreen);

	AddChildObject<Button>(1)->Register([buttonCallback]() {
		buttonCallback(1); // 移動
		}, Vector2(NODE_TAB_TEXT_POS.x + 100.0f, 35.0f), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "移動", L"asset\\texture\\alpha_texture.png", L"");

	fontData.Color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue);

	AddChildObject<Button>(1)->Register([buttonCallback]() {
		buttonCallback(2); // 死亡
		}, Vector2(NODE_TAB_TEXT_POS.x + 200.0f, 35.0f), Vector2(TAB_BUTTON_SIZE.x, TAB_BUTTON_SIZE.y), Vector2(0.0f, 0.0f), fontData, "死亡", L"asset\\texture\\alpha_texture.png", L"");

	// 右側の追加したいノード郡
	//Panel::AddChildObject<ImageDraw>(1)->Register(Vector3(950.0f, 50.0f, 0.0f), Vector3(400.0f, 70.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\debug_sprite.png");

	// 表示されたりされなかったりするなこれ->消してないのもあるし位置調整含めて後々でいいか
	// 描画されてない時、game_objのリストにはあるが範囲forにてヒットしておらず描画されない?
	AddChildObject<ImageDraw>(1)->Register(Vector3(PLAYER_NODE_LIST_POS.x, PLAYER_NODE_LIST_POS.y, 0.0f), 
		Vector3(PLAYER_NODE_LIST_SCALE.x, PLAYER_NODE_LIST_SCALE.y, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\player_node_list.png");

	AddChildObject<ImageDraw>(1)->Register(Vector3(ENEMY_NODE_LIST_POS.x, ENEMY_NODE_LIST_POS.y, 0.0f),
		Vector3(ENEMY_NODE_LIST_SCALE.x, ENEMY_NODE_LIST_SCALE.y, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\enemy_node_list.png");

	// 敵自体の見た目を敵ノード側に表示
	ImageDraw* image = AddChildObject<ImageDraw>(2); 
	image->Register(Vector3(ENEMY_VISUAL_POS.x, ENEMY_VISUAL_POS.y, 0.0f), Vector3(ENEMY_VISUAL_SCALE.x, ENEMY_VISUAL_SCALE.y, 0.0f), Vector3(0.0f, 0.0f, 0.0f), L"asset\\texture\\monsters.png");
	image->SetCanChangeVertex(true);
	image->ChangeTexUV(static_cast<float>(m_EnemyBase->GetTextureSplitCount().x), static_cast<float>(m_EnemyBase->GetTextureSplitCount().y), 
										   static_cast<float>(m_EnemyBase->GetUVPos().x), static_cast<float>(m_EnemyBase->GetUVPos().y), true);

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

	for(auto& child : GetChildObjectsByType<ImageDraw>())
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

void DnaScreenScript::GeneratePlayerVisualNodes()
{
	// プレイヤーに関しても所持しているノードの見た目部分を生成する
	Player* player = Manager::GetCurrentScene()->GetGameObject<Player>();
	int counter = 0;
	for(auto& node : player->GetAllNodes())
	{
		player->AddVisualNode(counter, node.get());
		counter++;
	}
}

void DnaScreenScript::SelectedAttackTab()
{
	// 現在のタブに対して移動反映を行う->これ必要なんだけどこれするとplayerのノードが消えてしまう
	GetActiveTab()->ApplyMovedResult();
	// プレイヤーノード再生成
	GeneratePlayerVisualNodes();

	GetChildObjectByType<Font>()->SetDisplayText("攻撃ノード");
	m_AttackVisual.SetIsSelected(true);
	m_AttackVisual.ModifyNodePos(); // ノード位置修正
	m_MoveVisual.SetIsSelected(false);
	m_DeathVisual.SetIsSelected(false);
}

void DnaScreenScript::SelectedMoveTab()
{
	// 現在のタブに対して移動反映を行う
	GetActiveTab()->ApplyMovedResult();

	GeneratePlayerVisualNodes();

	GetChildObjectByType<Font>()->SetDisplayText("移動ノード");
	m_AttackVisual.SetIsSelected(false);
	m_MoveVisual.SetIsSelected(true);
	m_MoveVisual.ModifyNodePos(); // ノード位置修正
	m_DeathVisual.SetIsSelected(false);
}

void DnaScreenScript::SelectedDeathTab()
{
	// 現在のタブに対して移動反映を行う
	GetActiveTab()->ApplyMovedResult();

	GeneratePlayerVisualNodes();

	GetChildObjectByType<Font>()->SetDisplayText("死亡ノード");
	m_AttackVisual.SetIsSelected(false);
	m_MoveVisual.SetIsSelected(false);
	m_DeathVisual.SetIsSelected(true);
	m_DeathVisual.ModifyNodePos(); // ノード位置修正
}