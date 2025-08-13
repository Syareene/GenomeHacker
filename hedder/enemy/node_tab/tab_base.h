#pragma once

//#include "object/2d_object.h"
#include "object/ui/button.h"
#include "enemy/node/base.h"

class TabBase : public Button
{
	// すべてのタブが継承するタブの基底クラス


	// 中身としては、
	// タブ部分の描画
	// 中に設置されているスクリプトのlist->中のリストに対して更新処理実行。
	// 背景の描画->dna_screen_scriptに情報を渡せば良い
	// タブの切り替え処理(button?)
	// んーこれbutton+αにしたほうが良さそげ、、

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Clicked(); // クリックされたときの処理
	std::list<NodeBase>& GetNodes() { return m_Nodes; } // 現在タブ内でくっついているノードのリストを取得
private:
	std::list<NodeBase> m_Nodes; // 現在タブ内でくっついているノードのリスト
	std::list<NodeBase> m_CanUseNodes; // タブ内で使用可能なノードのリスト(設置するための物、呼び出された際にプレイヤー側からソートしてここにいれる感じになるかな)
	// あと上記変数のめんどいところはすでに使われているか否かをどうにかしないといけない
	// プレイヤー側に実態は持っておいて、ここはあくまでポインタとして持っておき、実態にフラグ変数を設けることで管理しやすい、、、みたいな構造が望ましい。
};