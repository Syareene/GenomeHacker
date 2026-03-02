#include "main.h"
#include "enemy/node_tab/death.h"

void DeathTab::Init(const unsigned int& playerId, Transform trans)
{
	TabBase::Init(playerId, trans);
}

void DeathTab::ModifyTimeLine()
{
	// タイムラインを修正

	// クリア
	GetNodeTimeLineNotConst().clear();

	// ノードを前から取得し、前のcd+現在のcdを足す
	int currentTime = 0;
	for (const auto& node : GetNodes())
	{
		// 死亡タブの場合、InstantCastOnDeadがtrueならCDを0扱いに
		if (node->GetInstantCastOnDead())
		{
			GetNodeTimeLineNotConst().push_back(currentTime);
			node->SetCDMax(0); // CDMaxも0に
			continue;
		}
		// 通常加算
		currentTime += node->GetCDMax();
		GetNodeTimeLineNotConst().push_back(currentTime);
	}

	// 最後にm_CDMaxを更新
	SetCDMax(currentTime);
}
