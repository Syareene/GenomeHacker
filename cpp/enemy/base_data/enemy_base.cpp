// 敵データ基底クラス
// このデータはシーン側かなんかで常に持っといて、ノードの最新適応状態を見れるようにしておく、あとリソースも使い回せるので疑似flyweightとしても使用可能。
// 敵を出す際はこのデータのポインタを渡して出す感じになる。
// ノードに関してはdnaのボタンを押したときにこのクラスからデータを取ってくる形になるかな。

#include "enemy/base_data/enemy_base.h"

#include "enemy/node_tab/tab_base.h"
#include "enemy/node/base.h"
#include "enemy/node_tab/attack.h"
#include "enemy/node_tab/movement.h"
#include "enemy/node_tab/death.h"

void EnemyBase::Register()
{
	// 登録処理

	// そのenemy固有の情報を登録
	m_DnaScreen = std::make_unique<DnaScreenScript>();
	m_DnaScreen->Init(); // DNAスクリーンの初期化
	// タブが作られたので各ノードに対してenemyの初期ノードを登録しておく。
}

void EnemyBase::Unregister()
{
	// 登録解除処理
	if (m_DnaScreen)
	{
		m_DnaScreen->Uninit(); // DNAスクリーンの終了処理
		m_DnaScreen.reset(); // スクリーンのポインタを開放
	}
}

void EnemyBase::ExecuteAttack()
{
	// 攻撃ノードの処理を実行
	for (auto& node : m_DnaScreen->GetAttackTab()->GetNodes())
	{
		node.NodeEffect(); // ノードの効果を実行
	}
}

void EnemyBase::ExecuteMove()
{
	// 移動ノードの処理を実行
	for (auto& node : m_DnaScreen->GetMoveTab()->GetNodes())
	{
		node.NodeEffect(); // ノードの効果を実行
	}
}

bool EnemyBase::ExecuteDeath()
{
	// 死亡ノードの処理を実行
	for (auto& node : m_DnaScreen->GetDeathTab()->GetNodes())
	{
		node.NodeEffect();
	}
	return true; // 実行終わったらtrueを返す
}