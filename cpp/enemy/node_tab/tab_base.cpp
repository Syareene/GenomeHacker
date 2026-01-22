#include "main.h"
#include "enemy/node_tab/tab_base.h"
#include "lib/mouse.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"
#include <algorithm>

//unsigned int TabBase::m_PlayerId = 0; // プレイヤーポインタ初期化

void TabBase::Init(const unsigned int& playerId, Transform trans)
{
	
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
