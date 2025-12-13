#include "main.h"
#include "enemy/node_tab/tab_base.h"
#include "lib/mouse.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"

void TabBase::Init(Transform trans)
{
	SetTransform(trans);
	// タブの初期化処理
	m_IsSelected = false;

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

	if (m_IsSelected)
	{
		// タブ内にあるノードに対する更新処理
		for (auto& node : m_Nodes)
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
			for (int i = 0; i < static_cast<int>(m_Nodes.size()); i++)
			{
				if (&(*m_Nodes[i]) == node)
				{
					ModifyNodeIndexFromPos(Mouse::GetPosition(), i);
					break;
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
	}

	// タブの描画処理
	Button::Draw();
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
	// 座標加算用に保存
	float currentPosY = NODE_START.y;

	// index基準でnodeの位置を修正
	for (auto& node : m_Nodes)
	{
		// ノードの位置を修正
		Vector3 scale = node->GetScale();
		Vector2 diff = Vector2(NODE_START.x + (scale.x * 0.5f), currentPosY + (scale.y * 0.5f)) - Vector2(node->GetPosition().x, node->GetPosition().y);
		Vector3 old_pos = node->GetPosition();
		node->SetPosition(Vector3(old_pos.x + diff.x, old_pos.y + diff.y, old_pos.z));
		// 中身の説明文の位置も修正
		node->FixFontPositions(diff);

		// 次のノード用に位置を加算
		currentPosY += scale.y;
	}
}


// なぜかだいたい動いてしまったけど、動かしてるnodeが一番上とかの時(自身のindexの時?)に下に動かそうとすると動かないのでこの辺はなんとかしないとかも(中身が入れ替わってない?)
void TabBase::ModifyNodeIndexFromPos(Vector2 mousePos, int& grabIndex)
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
