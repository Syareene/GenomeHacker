#include "main.h"
#include "enemy/node_tab/tab_visual.h"
#include "enemy/base_data/enemy_base.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "object/player.h"
#include "enemy/dna_screen_script.h"
#include "lib/mouse.h"

#include <unordered_map>
#include <ranges>


// ノード入れ替え処理時、index基準でswapしつつposition修正かな。
void TabVisual::CreateVisual(TabBase* base)
{
	if(m_DnaScreenId == 0 || m_PlayerId == 0)
	{
		// (下の段階でidが設定されてるかわからないので暫定処理)
		assert(false && "TabVisual::CreateVisual called before IDs are set.");
		return;
	}

	int counter = 0;
	for (auto& node : base->GetNodes())
	{
		// visualbaseに必要なデータをnodeから引っ張ってくるようにして作成する形にする
		std::unique_ptr<VisualBase>& temp = m_VisualNodes.emplace_back(std::make_unique<VisualBase>());
		temp->Init(m_DnaScreenId, counter, node.get());
		counter++;
	}
}

void TabVisual::UpdateVisual(TabBase* base)
{
	for (auto& node : base->GetNodes())
	{
		// 中身見てフラグ変数がtrueならm_VisualNodesの対応する要素を更新する
	}
}


void TabVisual::Init(const unsigned int& screen_id, const unsigned int& player_id, TabBase* tab_base)
{
	// プレイヤーid保存
	m_PlayerId = player_id;
	// screen_id保存
	m_DnaScreenId = screen_id;
	m_Tab = tab_base;

	// 属するタブのノードを元に見た目を作成
	int counter = 0;
	for(auto& node : tab_base->GetNodes())
	{
		std::unique_ptr<VisualBase>& temp = m_VisualNodes.emplace_back(std::make_unique<VisualBase>());
		temp->Init(screen_id, counter, node.get());

		// setnamefont/adddescfont->改造して一括で渡せるように?

		counter++;
	}
}

void TabVisual::Uninit()
{
	if (!m_Tab)
	{
		return;
	}

	// ノードクリア
	m_VisualNodes.clear();

	// リセット
	m_Tab = nullptr;
}

void TabVisual::Update()
{
	if (m_IsSelected)
	{

		// タブ内にあるノードに対する更新処理
		for (auto& node : m_VisualNodes)
		{
			node->Update();
		}
		// プレイヤーが所持しているノードの更新処理
		Player* temp = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId);

		for (auto& node : temp->GetAllVisualNodes())
		{
			node->Update();
		}


		VisualBase* grabNode = Manager::GetCurrentScene()->GetCurrentState()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();
		// 該当するノードをm_Nodesから探してindexを取得
		if (grabNode)
		{
			ModifyEnemyNodeIndexFromPos(Mouse::GetPosition(), grabNode);
			ModifyPlayerNodeIndexFromPos(Mouse::GetPosition(), grabNode);
		}
	}
}

void TabVisual::Draw()
{
	if (m_IsSelected)
	{
		// タブ内にあるノードに対する描画処理
		for (auto& node : m_VisualNodes)
		{
			node->Draw();
		}

		// プレイヤーが所持しているノードの描画処理
		Player* temp = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId);
		for (auto& node : temp->GetAllVisualNodes())
		{
			node->Draw();
		}
	}
}


void TabVisual::ModifyNodePos()
{
	ModifyEnemyNodePos();
	ModifyPlayerNodePos();
}

void TabVisual::ApplyGrabNode()
{
	// 掴んでいるノードの位置を見て適切な場所に挿入
	VisualBase* grabNode = Manager::GetCurrentScene()->GetCurrentState()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();
	if (grabNode)
	{
		// 所属しているノードリストから一時変数にmove
		std::unique_ptr<VisualBase> tempNode;

		// enemyノード側から探す
		{
			auto it = std::find_if(m_VisualNodes.begin(), m_VisualNodes.end(),
				[&](const std::unique_ptr<VisualBase>& node) {
					return node.get() == grabNode;
				});
			if (it != m_VisualNodes.end())
			{
				// 見つかったらmoveで一時変数に保存
				tempNode = std::move(*it);
				// リストから削除 // (NodeBase更新版時追記: ここエラー出るかも?)
				m_VisualNodes.erase(it);
			}
		}

		// プレイヤーノード側から探す
		{
			auto& playerNodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllVisualNodes();
			auto it = std::find_if(playerNodes.begin(), playerNodes.end(),
				[&](const std::unique_ptr<VisualBase>& node) {
					return node.get() == grabNode;
				});
			if (it != playerNodes.end())
			{
				// 見つかったらmoveで一時変数に保存
				tempNode = std::move(*it);
				// リストから削除
				playerNodes.erase(it);
			}
		}

		// この段階でリストから消えているためgrabnodeのポインタを再度更新しないとエラーになる
		
		// grabNode を再設定する前に nullptr チェック
		if (tempNode)
		{
			Manager::GetCurrentScene()->GetCurrentState()->GetGameObject<DnaScreenScript>()->SetGrabbingNode(tempNode.get());
			grabNode = tempNode.get();
		}
		else
		{
			// tempNode が nullptr の場合、エラーを出力
			OutputDebugStringA("[Error] tempNode is nullptr in ApplyMovedResult\n");
			return;
		}

		// 掴みノードのx座標で敵ノードかプレイヤーノードかを判定
		if (grabNode->GetPosition().x < 768.0f)
		{
			// 敵ノード側に追加
			float grabPosY = grabNode->GetPosition().y;
			bool inserted = false;

			// iterator基準のループ処理
			for (auto it = m_VisualNodes.begin(); it != m_VisualNodes.end(); ++it)
			{
				if (grabPosY < (*it)->GetPosition().y)
				{
					// 見つかったら挿入
					m_VisualNodes.insert(it, std::move(tempNode));
					inserted = true;
					break;
				}
			}

			// ループ内で挿入されなかった場合、末尾に追加(イテレーターの範囲外になるのでpush_backでしか挿入できない)
			if (!inserted)
			{
				m_VisualNodes.push_back(std::move(tempNode));
			}

			ModifyEnemyNodePos();
		}
		else
		{
			// プレイヤーノード側に追加
			auto& playerNodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllVisualNodes();
			float grabPosY = grabNode->GetPosition().y;
			bool inserted = false;

			// iterator基準のループ処理
			for (auto it = playerNodes.begin(); it != playerNodes.end(); ++it)
			{
				if (grabPosY < (*it)->GetPosition().y)
				{
					// 見つかったら挿入
					playerNodes.insert(it, std::move(tempNode));
					inserted = true;
					break;
				}
			}

			// ループ内で挿入されなかった場合、末尾に追加(イテレーターの範囲外になるのでpush_backでしか挿入できない)
			if (!inserted)
			{
				playerNodes.push_back(std::move(tempNode));
			}

			ModifyPlayerNodePos();
		}
	}
}

// タブ移動時or編集stateからexitした時に呼び出す
void TabVisual::ApplyMovedResult()
{
	int index = 0;
	// 割り振り前にNodeBaseに対して整列用idを振る
	for(auto& node : m_Tab->GetNodes())
	{
		node->SetMoveManageId(index);
		index++;
	}
	index = 0;
	// プレイヤーのノードに対しても振る
	Player* temp = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId);
	for(auto& node : temp->GetAllNodes())
	{
		node->SetMoveManageId(index);
		index++;
	}
	// VisualBaseは生成時にid振ってあるので不要。

	index = 0;

	// 実行順序としては、、、
	// dnaに入った段階でNodeBaseとVisualBaseにidを割り振る(識別用として、元が同じindexからならidはおなじになる)
	// exit時にこの関数が実行される
	// NodeBaseのリストをenemy+playerでまとめる
	// EnemyのVisualのidを元にNodeBaseのリストと突き合わせてindexを修正する

	std::unordered_map<std::string, std::unique_ptr<NodeBase>> allNodes;

	// playerのデータを移動
	for(auto& node : temp->GetAllNodes())
	{
		if (node)
		{
			int id = node->GetMoveManageId();
			allNodes["p" + std::to_string(id)] = std::move(node);
		}
	}
	temp->GetAllNodes().clear();
	// enemyのデータを移動
	for(auto& node : m_Tab->GetNodes())
	{
		if (node)
		{
			int id = node->GetMoveManageId();
			allNodes["e" + std::to_string(id)] = std::move(node);
		}
	}
	m_Tab->GetNodes().clear();

	// player/enemyのVisualBaseを元にNodeBaseを再配置する

	// player
	for(const auto& v_node : temp->GetAllVisualNodes())
	{
		std::string key = "";
		if(v_node->GetNodeLocation() == NodeBase::NodeLocation::Player)
		{
			key = "p" + std::to_string(v_node->GetNodeBaseIndex());
		}
		else
		{
			key = "e" + std::to_string(v_node->GetNodeBaseIndex());
		}

		if(allNodes.count(key) > 0)
		{
			// 見つかったら移動
			temp->GetAllNodes().push_back(std::move(allNodes[key]));
			// 所在地変更
			temp->GetAllNodes().back()->SetNodeLocation(NodeBase::NodeLocation::Player);

			// 移動したのでmapから削除
			allNodes.erase(key);
		}
	}
	// enemy
	for(const auto& v_node : m_VisualNodes)
	{
		std::string key = "";
		if (v_node->GetNodeLocation() == NodeBase::NodeLocation::Player)
		{
			key = "p" + std::to_string(v_node->GetNodeBaseIndex());
		}
		else
		{
			key = "e" + std::to_string(v_node->GetNodeBaseIndex());
		}

		if(allNodes.count(key) > 0)
		{
			// 見つかったら移動
			m_Tab->GetNodes().push_back(std::move(allNodes[key]));
			// 所在地変更
			m_Tab->GetNodes().back()->SetNodeLocation(NodeBase::NodeLocation::Enemy);
			// 移動したのでmapから削除
			allNodes.erase(key);
		}
	}
	// 敵ノード側に対してTimeline更新関数を呼ぶ(ノードの更新が入ったため)
	m_Tab->ModifyTimeLine();

	// プレイヤーのVisualBaseは消す
	temp->GetAllVisualNodes().clear();

	return;
}

void TabVisual::ModifyEnemyNodePos(VisualBase* grabPtr)
{
	// 座標加算用に保存
	float currentPosY = ENEMY_NODE_START.y;
	// マウス座標を取得
	Vector2 mousePos = Mouse::GetPosition();

	bool isDownMove = false;
	// マウスの移動方向を見てっどっち側に動いてるかを判定
	Mouse::GetDiffPosition().y > 0.0f ? isDownMove = true : isDownMove = false;

	// index基準でnodeの位置を修正
	bool isOverGrabNode = false; // 掴みノードを超えたかどうか
	for (auto& node : m_VisualNodes)
	{
		// 掴みノードならマウス座標へ移動
		if (node.get() == grabPtr)
		{
			// ノードの位置を修正
			Vector3 old_pos = node->GetPosition();
			node->SetPosition(Vector3(mousePos.x, mousePos.y, old_pos.z));
			// 中身の説明文の位置も修正
			Vector2 diff = Vector2(mousePos.x, mousePos.y) - Vector2(old_pos.x, old_pos.y);
			node->FixFontPositions(diff);
			continue; // 次のノードへ
		}

		// 掴んでいるノードがある場合は、そのスペースを考慮する
		if (!isOverGrabNode && grabPtr)
		{
			// 掴んでいるノードが現在のノードよりも上にあるか、
			// または現在のノードの中心を掴んでいるノードの上端が超えた場合にスペースを空ける
			if (mousePos.y < node->GetPosition().y && mousePos.x <= ENEMY_AREA_END.x)
			{
				// 掴みノード分のスペースを確保
				currentPosY += grabPtr->GetScale().y;
				isOverGrabNode = true;
			}
		}

		// ノードの位置を修正
		Vector3 scale = node->GetScale();
		Vector2 diff = Vector2(ENEMY_NODE_START.x + (scale.x * 0.5f), currentPosY + (scale.y * 0.5f)) - Vector2(node->GetPosition().x, node->GetPosition().y);
		Vector3 old_pos = node->GetPosition();

		node->SetPosition(Vector3(old_pos.x + diff.x, old_pos.y + diff.y, old_pos.z));
		// 中身の説明文の位置も修正
		node->FixFontPositions(diff);

		// 次のノード用に位置を加算
		currentPosY += scale.y;
	}
}

void TabVisual::ModifyPlayerNodePos(VisualBase* grabPtr)
{
	// 座標加算用に保存
	float currentPosY = PLAYER_NODE_START.y;
	// マウス座標を取得
	Vector2 mousePos = Mouse::GetPosition();

	bool isDownMove = false;
	// マウスの移動方向を見てっどっち側に動いてるかを判定
	Mouse::GetDiffPosition().y > 0.0f ? isDownMove = true : isDownMove = false;

	// index基準でnodeの位置を修正
	bool isOverGrabNode = false; // 掴みノードを超えたかどうか
	for (auto& node : Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllVisualNodes())
	{
		// 掴みノードならマウス座標へ移動
		if (node.get() == grabPtr)
		{
			// ノードの位置を修正
			Vector3 old_pos = node->GetPosition();
			node->SetPosition(Vector3(mousePos.x, mousePos.y, old_pos.z));
			// 中身の説明文の位置も修正
			Vector2 diff = Vector2(mousePos.x, mousePos.y) - Vector2(old_pos.x, old_pos.y);
			node->FixFontPositions(diff);
			continue; // 次のノードへ
		}

		// 掴んでいるノードがある場合は、そのスペースを考慮する
		// ノードの基準が中心になっているがこれで行くしかないかな、ちらつきそうだし
		if (!isOverGrabNode && grabPtr)
		{
			// 掴んでいるノードが現在のノードよりも上にあるか、
			// または現在のノードの中心を掴んでいるノードの上端が超えた場合にスペースを空ける
			if (mousePos.y < node->GetPosition().y && mousePos.x > ENEMY_AREA_END.x)
			{
				// 掴みノード分のスペースを確保
				currentPosY += grabPtr->GetScale().y;
				isOverGrabNode = true;
			}
		}

		// ノードの位置を修正
		Vector3 scale = node->GetScale();
		Vector2 diff = Vector2(PLAYER_NODE_START.x + (scale.x * 0.5f), currentPosY + (scale.y * 0.5f)) - Vector2(node->GetPosition().x, node->GetPosition().y);
		Vector3 old_pos = node->GetPosition();

		node->SetPosition(Vector3(old_pos.x + diff.x, old_pos.y + diff.y, old_pos.z));
		// 中身の説明文の位置も修正
		node->FixFontPositions(diff);

		// 次のノード用に位置を加算
		currentPosY += scale.y;
	}
}

void TabVisual::ModifyEnemyNodeIndexFromPos(Vector2 mousePos, VisualBase* grabPtr)
{
	if (!grabPtr) return;

	// リストを走査
	for (auto& node : m_VisualNodes)
	{
		if (!grabPtr) continue;

		// 判定用の位置とサイズを取得
		Vector3 nodePos = node->GetPosition();
		Vector3 nodeScale = node->GetScale();

		// マウスがこのノード領域内にあるか判定
		if (mousePos.y < nodePos.y + (nodeScale.y * 0.5f) &&
			mousePos.y > nodePos.y - (nodeScale.y * 0.5f))
		{
			// 描画位置を再配置
			ModifyEnemyNodePos(grabPtr);

			return;
		}
	}
}

void TabVisual::ModifyPlayerNodeIndexFromPos(Vector2 mousePos, VisualBase* grabPtr)
{
	std::list<std::unique_ptr<VisualBase>>& all_nodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllVisualNodes();

	if (!grabPtr)
		return;

	// リストを走査
	for (auto& node : all_nodes)
	{
		if (node.get() == grabPtr)
			continue;

		VisualBase* curPtr = node.get();

		// 判定用の位置とサイズを取得
		Vector3 nodePos = curPtr->GetPosition();
		Vector3 nodeScale = curPtr->GetScale();

		// マウスがこのノード領域内にあるか判定
		if (mousePos.y < nodePos.y + (nodeScale.y * 0.5f) &&
			mousePos.y > nodePos.y - (nodeScale.y * 0.5f))
		{
			// 描画位置を再配置
			ModifyPlayerNodePos(grabPtr);

			return;
		}
	}
}