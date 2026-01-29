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

}