#pragma once

#include <list>
#include <memory>
#include "enemy/dna_screen_script.h"


class EnemyBase
{
public:
	void Register(); // 登録処理
	void Unregister(); // 登録解除処理
	// ノードの内容を下に行動させる関数(攻撃、動き、死亡時)
	void ExecuteAttack();
	void ExecuteMove();
	bool ExecuteDeath();

	// 敵リストタブ->DNAタブに遷移するボタンを表示するための関数->これボタンインスタンスを生成しないといけないから構造については考える必要あり
	// ただ関数としてはここにほしいかな
	void ShowToDnaButton(Vector2 pos);
	void HideToDnaButton();

	// DNAタブ関連の関数
	void ShowDnaScreen(); // 呼ばれたらコイツ自身のnode情報を持っているscriptを表示
	void HideDnaScreen();
private:
	std::unique_ptr<DnaScreenScript> m_DnaScreen; // dnaタブをまとめているpanel->初期化時に自身のnodeを持つために作成する必要あり
	// static -> ボタン押すときの外枠テクスチャ用変数? ただ元のボタンオブジェクトの仕組み的に複数テクスチャ対応してないからoverrideしないと無理やな 
};