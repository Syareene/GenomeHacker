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

	OutputDebugStringA("=== DnaEditState::Init START ===\n");

	// テクスチャの事前確認
	int textureId = TextureManager::LoadTexture(L"asset/texture/return_temp.png");
	if (textureId == -1)
	{
		assert(false);
	}

	// AddGameObject前のマネージャー状態確認
	auto& buttonsBeforeAdd = GetGameObjects<Button>();
	char beforeMsg[128];
	sprintf_s(beforeMsg, "Before AddGameObject - Button count: %zu\n", buttonsBeforeAdd.size());
	OutputDebugStringA(beforeMsg);

	Button* button_ptr = AddGameObject<Button>(0);
	if (!button_ptr)
	{
		assert(false);
	}

	// AddGameObject直後の状態確認
	auto& buttonsAfterAdd = GetGameObjects<Button>();
	char afterAddMsg[128];
	sprintf_s(afterAddMsg, "After AddGameObject - Button count: %zu\n", buttonsAfterAdd.size());
	OutputDebugStringA(afterAddMsg);

	// Register前の詳細確認
	OutputDebugStringA("Before Register\n");

	try 
	{
		button_ptr->Register([]()
			{
				auto currentState = Manager::GetCurrentScene()->GetCurrentState();
				if (auto dnaEditState = dynamic_cast<DnaEditState*>(currentState))
				{
					OutputDebugStringA("Button clicked successfully!\n");
					dnaEditState->Clicked();
				}
			}, Vector2(125.0f, 40.0f), Vector2(250.0f, 80.0f), Vector2(0.0f, 0.0f), textureId);

		OutputDebugStringA("Register completed successfully\n");
	}
	catch (const std::exception& e) 
	{
		OutputDebugStringA("Register failed with exception\n");
		assert(false);
	}

	// Register後の状態確認
	auto& buttonsAfterRegister = GetGameObjects<Button>();
	char afterRegMsg[128];
	sprintf_s(afterRegMsg, "After Register - Button count: %zu\n", buttonsAfterRegister.size());
	OutputDebugStringA(afterRegMsg);

	// FlushPendingObjects実行
	FlushPendingObjects();

	// Flush後の最終確認
	auto& buttonsAfterFlush = GetGameObjects<Button>();
	char afterFlushMsg[128];
	sprintf_s(afterFlushMsg, "After FlushPendingObjects - Button count: %zu\n", buttonsAfterFlush.size());
	OutputDebugStringA(afterFlushMsg);

	// ボタンの詳細情報
	if (!buttonsAfterFlush.empty())
	{
		Button& btn = buttonsAfterFlush[0];
		char detailMsg[256];
		sprintf_s(detailMsg, "Button details - ID: %u, Active: %d, Destroy: %d, Pos: (%.1f, %.1f)\n",
			btn.GetObjectID(), btn.IsActive(), btn.IsDestroy(), 
			btn.GetPosition().x, btn.GetPosition().y);
		OutputDebugStringA(detailMsg);
	}

	OutputDebugStringA("=== DnaEditState::Init END ===\n");
}

void DnaEditState::Uninit()
{
	// override後、個別で追加したいものの追加
	State::Uninit();
}

void DnaEditState::Update()
{
	State::Update();

	// 定期的なオブジェクト数確認
	static int frameCount = 0;
	if (++frameCount % 120 == 0) // 2秒に1回
	{
		auto& buttons = GetGameObjects<Button>();
		char updateMsg[128];
		sprintf_s(updateMsg, "Update check - Button count: %zu\n", buttons.size());
		OutputDebugStringA(updateMsg);
		
		for (size_t i = 0; i < buttons.size(); ++i)
		{
			char btnMsg[256];
			sprintf_s(btnMsg, "  Button[%zu] - ID: %u, Active: %d, Destroy: %d\n",
				i, buttons[i].GetObjectID(), buttons[i].IsActive(), buttons[i].IsDestroy());
			OutputDebugStringA(btnMsg);
		}
	}

	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
	UpdateStateObject();
	// ノード掴み処理の適用
	//ApplyGrabNode();
}

void DnaEditState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTag("dna_edit");

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


// tab_baseのModifyNodeIndexFromPosみたいなしょうに関数作ってその中でこの関数実行するようにする
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

//void DnaEditState::ApplyGrabNode()
//{
//	if (m_IsReleaseGrabNode)
//	{
//		// もし掴んでるノードがあるなら挿入処理関数を実行
//		if (m_GrabbingNode)
//		{
//			auto tab = m_CurrentEnemyBase->GetDnaScreen()->GetActiveTab();
//			if (tab)
//			{
//				tab->ApplyGrabNode();
//			}
//		}
//		m_GrabbingNode = nullptr; // 掴んでいるノードを離す
//		m_IsReleaseGrabNode = false;
//	}
//}

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