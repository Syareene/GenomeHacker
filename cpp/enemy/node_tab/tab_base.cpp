#include "main.h"
#include "enemy/node_tab/tab_base.h"
#include "lib/mouse.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"
#include <algorithm>

unsigned int TabBase::m_PlayerId = 0; // プレイヤーポインタ初期化

void TabBase::Init(const unsigned int& playerId, Transform trans)
{
	SetTransform(trans);
	// タブの初期化処理
	m_IsSelected = false;
	m_PlayerId = playerId;

	Button::Init();
	
}

void TabBase::Update()
{

	// m_IsSelectedがtrueのときにのみ子ノードに対するマウス選択処理等々を行う(描画も同じ)
	// ただこの場合次のノードに移行するみたいな処理をどうするかっすね


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

// 掴みも含めて現在のノードのindex修正する関数必要やなこれ
// 管理はこっちだからこっちで実行したいけどマウス操作が個別ノード側なんだよな
// 掴んでいる状態なら掴み状態の位置をチェックして超えてるindexがあれば移動みたいな

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
