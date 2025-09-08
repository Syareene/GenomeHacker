#pragma once

//#include "object/2d_object.h"
#include "object/ui/button.h"
#include "enemy/node/base.h"
#include <list>
#include <memory>

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
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual void Clicked(); // クリックされたときの処理
	std::list<std::unique_ptr<NodeBase>>& GetNodes() { return m_Nodes; } // 現在タブ内でくっついているノードのリストを取得
	template <typename T>
	T* AddNode(int index) // ノードを追加
	{
		// indexは-1の場合最後尾へ、そうでない場合は任意の位置へ。

		return nullptr;
	};
private:
	std::list<std::unique_ptr<NodeBase>> m_Nodes; // 現在タブ内でくっついているノードのリスト
	std::list<NodeBase> m_CanUseNodes; // タブ内で使用可能なノードのリスト(設置するための物、呼び出された際にプレイヤー側からソートしてここにいれる感じになるかな)
	// あと上記変数のめんどいところはすでに使われているか否かをどうにかしないといけない
	// プレイヤー側に実態は持っておいて、ここはあくまでポインタとして持っておき、実態にフラグ変数を設けることで管理しやすい、、、みたいな構造が望ましい。
};