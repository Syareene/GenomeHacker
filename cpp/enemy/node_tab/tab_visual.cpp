#include "main.h"
#include "enemy/node_tab/tab_visual.h"
#include "enemy/base_data/enemy_base.h"
#include "scene/manager.h"
#include "player.h"
#include "enemy/dna_screen_script.h"
#include "lib/mouse.h"


// ノード入れ替え処理時、index基準でswapしつつposition修正かな。


void TabVisual::CreateVisual(TabBase* base)
{
	for (auto& node : base->GetNodes())
	{
		// まだ実装してないが、visualbaseに必要なデータをnodeから引っ張ってくるようにして作成する形にする
		m_VisualNodes.push_back(VisualBase());
		m_VisualNodes.back().Init(this, node.get());
	}

	// プレイヤーのvisualも生成?->player側で生成し持っておくor dna_screen側で持つ?
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

	// 属するタブのノードを元に見た目を作成
	for(auto& node : tab_base->GetNodes())
	{
		m_VisualNodes.push_back(VisualBase());
		// 最後尾に対してnodeを元に初期化する
		// nodeのptr渡してあげるのが一番いいかもね
		m_VisualNodes.back().Init(this, node.get());

		// setnamefont/adddescfont->改造して一括で渡せるように?
	}
}

void TabVisual::Uninit()
{

}

void TabVisual::Update()
{
	// 更新時dna_screen_scriptからplayer_idをもらう
	// でもpanel継承だからupdateの引数変えられないし、保存するしかないかねぇ、考えないとダメだこれ


	// タブ内にあるノードに対する更新処理
	for (auto& node : m_VisualNodes)
	{
		node.Update();
	}
	// プレイヤーが所持しているノードの更新処理
	Player* temp = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId);

	// これVisualBaseに変換しないと描画できないな、、ｗ困ったぞ

	for (auto& node : temp->GetAllVisualNodes())
	{
		node.Update();
	}


	VisualBase* grabNode = Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();
	// 該当するノードをm_Nodesから探してindexを取得
	if (grabNode)
	{
		ModifyEnemyNodeIndexFromPos(Mouse::GetPosition(), grabNode);
		ModifyPlayerNodeIndexFromPos(Mouse::GetPosition(), grabNode);
	}
}

void TabVisual::Draw()
{
	// タブ内にあるノードに対する描画処理
	for (auto& node : m_VisualNodes)
	{
		node.Draw();
	}

	// プレイヤーが所持しているノードの描画処理
	Player* temp = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId);
	for (auto& node : temp->GetAllVisualNodes())
	{
		node.Draw();
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
	VisualBase* grabNode = Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();
	if (grabNode)
	{
		// 掴みノードがenemy/playerどっちに所属しているかを取得
		NodeBase::NodeLocation loc = grabNode->GetNodeLocation();

		// 所属しているノードリストから一時変数にmove
		std::unique_ptr<NodeBase> tempNode = nullptr;
		if (loc == NodeBase::NodeLocation::Enemy)
		{
			// enemyノード側から探す
			auto it = std::find_if(m_VisualNodes.begin(), m_VisualNodes.end(),
				[&](const std::unique_ptr<VisualBase>& node) {
					return node.get() == grabNode;
				});
			if (it != m_VisualNodes.end())
			{
				// 見つかったらmoveで一時変数に保存
				tempNode = std::move(*it);
				// リストから削除
				m_VisualNodes.erase(it);
			}
		}
		else if (loc == NodeBase::NodeLocation::Player)
		{
			// プレイヤーノード側から探す
			auto& playerNodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllNodes();
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

		// 探索しても何も無い場合終了
		if (!tempNode)
		{
			return;
		}


		// この段階でリストから消えているためgrabnodeのポインタを再度更新しないとエラーになる
		Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->SetGrabbingNode(tempNode.get());
		grabNode = tempNode.get();
		//grabNode = state->GetGrabbingNode();

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
			auto& playerNodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllNodes();
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
				// プレイヤーに対してノードを追加する関数が必要かも
				playerNodes.push_back(std::move(tempNode));
			}

			ModifyPlayerNodePos();
		}
	}
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
		// 上から下は動いてるけど下から上に動く時1つ分ずれちゃってるね

		// 掴みノードならマウス座標へ移動
		if (node == grabPtr)
		{
			// ノードの位置を修正
			Vector3 old_pos = node.GetPosition();
			node.SetPosition(Vector3(mousePos.x, mousePos.y, old_pos.z));
			// 中身の説明文の位置も修正
			Vector2 diff = Vector2(mousePos.x, mousePos.y) - Vector2(old_pos.x, old_pos.y);
			node.FixFontPositions(diff);
			continue; // 次のノードへ
		}

		// 掴んでいるノードがある場合は、そのスペースを考慮する
		// ノードの基準が中心になっているが一旦はこれで行くしかないかな、ちらつきそうだし
		if (!isOverGrabNode && grabPtr)
		{
			// 掴んでいるノードが現在のノードよりも上にあるか、
			// または現在のノードの中心を掴んでいるノードの上端が超えた場合にスペースを空ける
			if (mousePos.y < node.GetPosition().y && mousePos.x <= ENEMY_AREA_END.x)
			{
				// 掴みノード分のスペースを確保
				currentPosY += grabPtr->GetScale().y;
				isOverGrabNode = true;
			}
		}

		// ノードの位置を修正
		Vector3 scale = node.GetScale();
		Vector2 diff = Vector2(ENEMY_NODE_START.x + (scale.x * 0.5f), currentPosY + (scale.y * 0.5f)) - Vector2(node.GetPosition().x, node.GetPosition().y);
		Vector3 old_pos = node.GetPosition();

		node.SetPosition(Vector3(old_pos.x + diff.x, old_pos.y + diff.y, old_pos.z));
		// 中身の説明文の位置も修正
		node.FixFontPositions(diff);

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
	for (auto& node : Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllNodes())
	{
		// 上から下は動いてるけど下から上に動く時1つ分ずれちゃってるね

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
		// ノードの基準が中心になっているが一旦はこれで行くしかないかな、ちらつきそうだし
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


// なぜかだいたい動いてしまったけど、動かしてるnodeが一番上とかの時(自身のindexの時?)に下に動かそうとすると動かないのでこの辺はなんとかしないとかも(中身が入れ替わってない?)
void TabVisual::ModifyEnemyNodeIndexFromPos(Vector2 mousePos, VisualBase* grabPtr)
{
	// バグはなくなったが、現在のindexの範囲の場合nodeが動かないようにしてマウス座標だけ動きマウス座標がその範囲からでたら動くような形に変更したほうがいいかな
	// ->ちゃんとindexが変わるようになりそれベースで位置変えてるので動かしたら強制的に位置が変わるようになっているのが原因

	if (!grabPtr) return;

	// リストを走査
	for (auto& node : m_VisualNodes)
	{
		if (!grabPtr) continue;

		// 判定用の位置とサイズを取得
		Vector3 nodePos = node.GetPosition();
		Vector3 nodeScale = node.GetScale();

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
	std::list<std::unique_ptr<NodeBase>>& all_nodes = Manager::GetCurrentScene()->GetGameObjectById<Player>(m_PlayerId)->GetAllNodes();

	if (!grabPtr)
		return;

	// リストを走査
	for (auto& node : all_nodes)
	{
		if (node.get() == grabPtr)
			continue;

		NodeBase* curPtr = node.get();

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