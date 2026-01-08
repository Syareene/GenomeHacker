#include "main.h"
#include "scene/state/dna_edit_state.h"
#include "scene/state/dna_table_state.h"
#include "scene/manager.h"
#include "object/ui/button.h"
#include "manager/texture_manager.h"
#include "enemy/base_data/enemy_base.h"

#include "enemy/node/base.h"

void DnaEditState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);

	State::Init();

	AddGameObject<Button>(0)->Register([this]() {
		// ボタンがクリックされた時の処理
			Clicked();
			}, Vector2(125.0f, 40.0f), Vector2(250.0f, 80.0f), Vector2(0.0f, 0.0f), TextureManager::LoadTexture(L"asset/texture/return_temp.png"));

	//m_Player = Manager::GetCurrentScene()->GetGameObject<Player>();
	//if (!m_Player)
	//{
	//	// 取得できなければエラーを出す
	//	assert(false && "DnaEditState::Init() -> m_Player is nullptr");
	//}

	// 大本のSceneの
	//Manager::GetCurrentScene()->Init();
}

void DnaEditState::Uninit()
{
	// 掴んでいるノードを離す
	m_GrabbingNode = nullptr;

	// override後、個別で追加したいものの追加
	State::Uninit();
}

void DnaEditState::Update()
{
	State::Update();
	// このstateはとりあえず通常更新+このstate限定オブジェクトの更新

	// 大本のsceneの更新
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");

	m_CurrentEnemyBase->GetDnaScreen()->Update();

	// ノード更新前にプレイヤーが所有しているノードの位置を修正
	//ModifyNodePos();

	//for(auto& node : m_Player->GetAllNodes())
	//{
	//	// ノードの更新
	//	node->Update();
	//}

	// state内オブジェクトの更新
	//UpdateStateObjectByTag("dna_edit");
	UpdateStateObject();
	// ノード掴み処理の適用
	ApplyGrabNode();
	DeleteGameObject();
}

void DnaEditState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
	m_CurrentEnemyBase->GetDnaScreen()->Draw();

	//for (auto& node : m_Player->GetAllNodes())
	//{
	//	// ノードの更新
	//	node->Draw();
	//}
	// state内オブジェクトの描画
	DrawStateObject();

	State::UpdateFinal();
}

void DnaEditState::Clicked()
{
	// state変更
	Manager::GetCurrentScene()->SetState<DnaTableState>();
}


// tab_baseのModifyNodeIndexFromPosみたいな関数作ってその中でこの関数実行するようにする
// んでそしたらちゃんと動く
void DnaEditState::ModifyNodePos()
{
	// プレイヤーが持った時に動かないようにする処理は追加しなければならない

	// 座標加算用に保存
	float currentPosY = NODE_START_POS.y;

	// index基準でnodeの位置を修正
	for (int i = 0; true; i++)
	{
		//NodeBase* node = m_Player->GetNodeByIndex(i);
		//// もしnullptrなら終了
		//if(node == nullptr)
		//{
		//	break;
		//}

		//// ノードの位置を修正
		//Vector3 scale = node->GetScale();
		//Vector2 diff = Vector2(NODE_START_POS.x + (scale.x * 0.5f), currentPosY + (scale.y * 0.5f)) - Vector2(node->GetPosition().x, node->GetPosition().y);
		//Vector3 old_pos = node->GetPosition();
		//// 次のノード用に位置を加算
		//currentPosY += scale.y;

		//if (m_GrabbingNode == node)
		//{
		//	// 掴んでいるノードは位置を変更しない
		//	continue;
		//}
		//node->SetPosition(Vector3(old_pos.x + diff.x, old_pos.y + diff.y, old_pos.z));
		//// 中身の説明文の位置も修正
		//node->FixFontPositions(diff);
	}
}

void DnaEditState::ApplyGrabNode()
{
	if (m_IsReleaseGrabNode)
	{
		// もし掴んでるノードがあるなら挿入処理関数を実行
		if (m_GrabbingNode)
		{
			auto tab = m_CurrentEnemyBase->GetDnaScreen()->GetActiveTab();
			if (tab)
			{
				tab->ApplyGrabNode();
			}
		}
		m_GrabbingNode = nullptr; // 掴んでいるノードを離す
		m_IsReleaseGrabNode = false;
	}
}

/*
void DnaEditState::ModifyEnemyNodeIndexFromPos(Vector2 mousePos, int& grabIndex)
{
	// バグはなくなったが、現在のindexの範囲の場合nodeが動かないようにしてマウス座標だけ動きマウス座標がその範囲からでたら動くような形に変更したほうがいいかな
	// ->ちゃんとindexが変わるようになりそれベースで位置変えてるので動かしたら強制的に位置が変わるようになっているのが原因

	if (grabIndex < 0 || grabIndex >= static_cast<int>(m_Nodes.size()))
		return;

	// 安全のため生ポインタを保持（デバッグ用にも使える）
	NodeBase* grabbedPtr = m_Nodes[grabIndex].get();

	// シンプルかつ安定した実装：候補ノードとインデックスを交換する（swap）
	// erase/insert を使うよりもポインタの移動とインデックス更新が確実
	for (size_t i = 0; i < m_Nodes.size(); ++i)
	{
		if (static_cast<int>(i) == grabIndex)
			continue;

		NodeBase* curPtr = m_Nodes[i].get();

		// 判定用の位置とサイズを取得
		Vector3 nodePos = curPtr->GetPosition();
		Vector3 nodeScale = curPtr->GetScale();

		// マウスがこのノード領域内にあるか判定
		if (mousePos.y < nodePos.y + (nodeScale.y * 0.5f) &&
			mousePos.y > nodePos.y - (nodeScale.y * 0.5f))
		{
			// インデックス交換（swap）は簡潔で安全
			std::swap(m_Nodes[grabIndex], m_Nodes[i]);

			// grabIndex を移動先に合わせて更新
			grabIndex = static_cast<int>(i);

			// 描画位置を再配置
			ModifyNodePos();

			return;
		}
	}
}
*/