#include "main.h"
#include "enemy/node_tab/tab_base.h"
#include "lib/mouse.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"
#include <algorithm>

Player* TabBase::m_PlayerPtr = nullptr; // プレイヤーポインタ初期化

void TabBase::Init(Transform trans)
{
	SetTransform(trans);
	// タブの初期化処理
	m_IsSelected = false;

	// playerptrがnullなら取得
	if(!m_PlayerPtr)
	{
		m_PlayerPtr = Manager::GetCurrentScene()->GetGameObject<Player>();
		if (!m_PlayerPtr)
		{
			// エラー
			assert(false && "TabBase::Init() -> Failed to get Player pointer.");
		}
	}

	Button::Init();
	
}

void TabBase::Uninit()
{
	// タブの終了処理
	Button::Uninit();
	
}

void TabBase::Update()
{
	// m_IsSelectedがtrueのときにのみ子ノードに対するマウス選択処理等々を行う(描画も同じ)
	// ただこの場合次のノードに移行するみたいな処理をどうするかっすね


	// m_IsSelectedの更新範囲もうちょい広くしないとダメだね(最小限の更新処理だけに留めるべき)
	if (m_IsSelected)
	{
		// タブ内にあるノードに対する更新処理
		for (auto& node : m_Nodes)
		{
			node->Update();
		}
		// プレイヤーが所持しているノードの更新処理
		for(auto& node : m_PlayerPtr->GetAllNodes())
		{
			node->Update();
		}
	}

	// 掴んでるノードを取得
	DnaEditState* state = static_cast<DnaEditState*>(Manager::GetCurrentScene()->GetStatePtr());
	if (state)
	{
		NodeBase* node = state->GetGrabbingNode();

		// 該当するノードをm_Nodesから探してindexを取得
		if (node)
		{
			bool passed = false;
			int count = 0;


			for (auto& node : m_Nodes)
			{
				if (node.get() == state->GetGrabbingNode())
				{
					ModifyEnemyNodeIndexFromPos(Mouse::GetPosition(), count);
					passed = true;
					break;
				}
				count++;
			}

			// プレイヤーのノードも対象に走査
			count = 0;
			if (!passed)
			{
				for(auto& player_node : m_PlayerPtr->GetAllNodes())
				{
					if (&(*player_node) == node)
					{
						// この関数内でplayer/enemyによって呼ぶ先分けるように
						ModifyPlayerNodeIndexFromPos(Mouse::GetPosition(), count); // プレイヤーのノードはtab内のノードではないのでindex変更しない
						break;
					}
					count++;
				}
			}
		}
	}


	// タブの更新処理
	Button::Update();
	// タブ内にあるノードに対する更新処理

	// タブ内のノードに関しては順番に実行するからこっちで実行しているindexもっておいて
	// nodeeffect実行したときに内部でカウント持っておいてcd終わってこいつが実行されたらindexを進めるみたいな形で対応すればいいんじゃないかな

	// 実行


	// 一旦エラー解消用にコメントアウト
	/*
	auto it = m_Nodes.begin();
	std::advance(it, m_Index);
	if (it->get()->NodeEffect(this))
	{
		// cd終わったので次のノードを参照
		m_Index++;
		// 範囲外なら最初に戻る
		if(m_Index >= static_cast<int>(m_Nodes.size()))
		{
			m_Index = 0; // 最初に戻る
		}
	}
	*/

	// ex
	// cd120f, atk1
	// cd60f, atk2
	// だとしたらcdの更新状態は内部で持っておいて、今実行したいindexをこのクラスで持っておく感じ、んでそのindex番目に実行。
}

void TabBase::Draw()
{
	if (m_IsSelected)
	{
		// タブ内にあるノードに対する描画処理
		for (auto& node : m_Nodes)
		{
			node->Draw();
		}

		// プレイヤーが所持しているノードの描画処理
		for (auto& node : m_PlayerPtr->GetAllNodes())
		{
			node->Draw();
		}

		// タブの描画処理
		Button::Draw();
	}

	// タブ内にあるノードに対する描画処理

	// 自身の描画
}

void TabBase::Clicked()
{
	// クリックされたときの処理
	// 例えばタブの切り替えとか
}

// 掴みも含めて現在のノードのindex修正する関数必要やなこれ
// 管理はこっちだからこっちで実行したいけどマウス操作が個別ノード側なんだよな
// 掴んでいる状態なら掴み状態の位置をチェックして超えてるindexがあれば移動みたいな

void TabBase::ModifyNodePos()
{
		ModifyEnemyNodePos();
		ModifyPlayerNodePos();
}

void TabBase::ApplyGrabNode()
{
	// 掴んでいるノードの位置を見て適切な場所に挿入
	DnaEditState* state = static_cast<DnaEditState*>(Manager::GetCurrentScene()->GetStatePtr());
	if (state)
	{
		NodeBase* grabNode = state->GetGrabbingNode();
		if (grabNode)
		{
			// 掴みノードがenemy/playerどっちに所属しているかを取得
			NodeBase::NodeLocation loc = grabNode->GetNodeLocation();

			// 所属しているノードリストから一時変数にmove
			std::unique_ptr<NodeBase> tempNode = nullptr;
			if (loc == NodeBase::NodeLocation::Enemy)
			{
				// enemyノード側から探す
				auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(),
					[&](const std::unique_ptr<NodeBase>& node) {
						return node.get() == grabNode;
					});
				if (it != m_Nodes.end())
				{
					// 見つかったらmoveで一時変数に保存
					tempNode = std::move(*it);
					// リストから削除
					m_Nodes.erase(it);
				}
			}
			else if (loc == NodeBase::NodeLocation::Player)
			{
				// プレイヤーノード側から探す
				auto& playerNodes = m_PlayerPtr->GetAllNodes();
				auto it = std::find_if(playerNodes.begin(), playerNodes.end(),
					[&](const std::unique_ptr<NodeBase>& node) {
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
			state->SetGrabbingNode(tempNode.get());
			grabNode = state->GetGrabbingNode();

			// 掴みノードのx座標で敵ノードかプレイヤーノードかを判定
			if (grabNode->GetPosition().x < 768.0f)
			{
				// 敵ノード側に追加
				float grabPosY = grabNode->GetPosition().y;
				bool inserted = false;

				// iterator基準のループ処理
				for (auto it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
				{
					if (grabPosY < (*it)->GetPosition().y)
					{
						// 見つかったら挿入
						m_Nodes.insert(it, std::move(tempNode));
						inserted = true;
						break;
					}
				}

				// ループ内で挿入されなかった場合、末尾に追加(イテレーターの範囲外になるのでpush_backでしか挿入できない)
				if (!inserted)
				{
					m_Nodes.push_back(std::move(tempNode));
				}

				ModifyEnemyNodePos();
			}
			else
			{
				// プレイヤーノード側に追加
				auto& playerNodes = m_PlayerPtr->GetAllNodes();
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
}

void TabBase::ModifyEnemyNodePos(NodeBase* grabPtr)
{
	// 座標加算用に保存
	float currentPosY = ENEMY_NODE_START.y;
	// マウス座標を取得
	Vector2 mousePos = Mouse::GetPosition();

	bool isDownMove = false;
	// マウスの移動方向を見てっどっち側に動いてるかを判定
	Mouse::GetDiffPosition().y > 0.0f ? isDownMove = true : isDownMove = false;
	if (grabPtr)
	{
		// 掴んでるノードのposとscaleを取得
		Vector2 grabNodePos = Vector2(grabPtr->GetPosition().x, grabPtr->GetPosition().y);
		Vector3 grabNodeScale = grabPtr->GetScale();
		// 掴みノードの開始yを計算(マウス座標からscaleの半分引いた位置)
	}

	// index基準でnodeの位置を修正
	bool isOverGrabNode = false; // 掴みノードを超えたかどうか
	for (auto& node : m_Nodes)
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
			if (mousePos.y < node->GetPosition().y)
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

void TabBase::ModifyPlayerNodePos(NodeBase* grabPtr)
{
	// 座標加算用に保存
	float currentPosY = PLAYER_NODE_START.y;

	// index基準でnodeの位置を修正
	for (auto& node : m_PlayerPtr->GetAllNodes())
	{
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
void TabBase::ModifyEnemyNodeIndexFromPos(Vector2 mousePos, int& grabIndex)
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
			//std::swap(m_Nodes[grabIndex], m_Nodes[i]);

			// grabIndex を移動先に合わせて更新
			//grabIndex = static_cast<int>(i);

			// これの呼び出し方が違うかも?curPtr->grabbedPTr

			// 描画位置を再配置
			ModifyEnemyNodePos(grabbedPtr);

			return;
		}
	}
}

void TabBase::ModifyPlayerNodeIndexFromPos(Vector2 mousePos, int& grabIndex)
{
	std::list<std::unique_ptr<NodeBase>>& all_nodes = m_PlayerPtr->GetAllNodes();

	if (grabIndex < 0 || grabIndex >= static_cast<int>(all_nodes.size()))
		return;

	// 安全のため生ポインタを保持（デバッグ用にも使える）
	auto it = all_nodes.begin();
	std::advance(it, grabIndex);
	NodeBase* grabbedPtr = it->get();

	// シンプルかつ安定した実装：候補ノードとインデックスを交換する（swap）
	// erase/insert を使うよりもポインタの移動とインデックス更新が確実
	for (size_t i = 0; i < all_nodes.size(); ++i)
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
			ModifyPlayerNodePos(curPtr);

			return;
		}
	}
}

void TabBase::ModifyTimeLine()
{
	// タイムラインを修正

	// クリア
	m_NodeTimeLine.clear();

	// ノードを前から取得し、前のcd+現在のcdを足す
	int currentTime = 0;
	for (const auto& node : m_Nodes)
	{
		currentTime += node->GetCDMax();
		m_NodeTimeLine.push_back(currentTime);
	}

	// 最後にm_CDMaxを更新
	m_CDMax = currentTime;
	if(m_CDMax == 0)
	{
		m_CDMax = 1; // 0だと困るので1に
	}
}
