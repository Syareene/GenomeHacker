#include "main.h"
#include "enemy/node_tab/tab_base.h"

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
