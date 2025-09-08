#include "object/ui/in_game/enemy_node.h"
#include "object/game_object.h"

void EnemyNode::Init(Transform trans)
{
	SetTransform(trans);
	// 置きたいオブジェクトを軒並みセットする
	

	// enemyのノードのタブすべて(タブのuiと、タブ内部の情報と。)
	// 現在のタブの情報->その情報を下にそのタブを開く処理を実施
	// 戻るボタン
	// あくまで包容するだけなのでそのオブジェクトに対してのアップデート処理を実行する必要はない。
}

void EnemyNode::Uninit()
{
	
}

void EnemyNode::Update()
{
	
}

void EnemyNode::Draw()
{
	
}

void EnemyNode::Activate()
{
	// ゲーム中のボタンから開く処理
	
	// このボタン押したらゲーム側だけ止めないといけないが。ゲーム側のオブジェクトのactiveをfalseにするか?
	// ないしはstate
}

void EnemyNode::Deactivate()
{
	// タブ内の戻るボタンを押しゲームに戻る処理
}