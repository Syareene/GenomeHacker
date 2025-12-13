#pragma once

#include "object/ui/button.h"
#include "enemy/node/base.h"
#include <vector>
#include <memory>

template<typename T>
concept NodeType = std::is_base_of_v<NodeBase, T>;

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
	// index基準でnodeの位置を修正
	void ModifyNodePos();
	// ノード掴んでる時に掴んだノード基準でtab内のnode見て見た目含めindexを修正
	void ModifyNodeIndexFromPos(Vector2 mousePos, int& grabIndex);
	inline void SetIsSelected(const bool isSelected) { m_IsSelected = isSelected; } // 現在選択されているタブかどうかを設定
	inline const bool GetIsSelected() const { return m_IsSelected; } // 現在選択されているタブかどうかを取得
	std::vector<std::unique_ptr<NodeBase>>& GetNodes() { return m_Nodes; } // 現在タブ内でくっついているノードのリストを取得
	inline const int GetCDMax() const { return m_CDMax; } // タブ内にあるノードをすべて合計したクールダウンを取得
	inline const std::list<int>& GetNodeTimeLine() const { return m_NodeTimeLine; } // タブ内にあるノードのcdが終わるタイミングを開始時から数えたときのリストを取得
	template <NodeType T>
	T* AddNode(const int& index, Transform trans = Transform()) // ノードを追加
	{
		// indexは-1の場合最後尾へ、そうでない場合は任意の位置へ。

		if (index < -1 || index > static_cast<int>(m_Nodes.size()))
		{
			// 範囲外
			return nullptr;
		}

		// インスタンス作る(これmove必要にはなっちゃうからmoveコンストラクタをそのうち作る必要あり)
		std::unique_ptr<T> newNode = std::make_unique<T>();
		// 初期化
		newNode.get()->Init(trans);
		NodeBase* upperNode = nullptr;
		NodeBase* lowerNode = nullptr;
		// 上側ノード挿入判定
		if(index > 0)
		{
			upperNode = m_Nodes[index - 1].get();
		}
		// 下側ノード挿入判定
		if (index < static_cast<int>(m_Nodes.size()) && index != -1)
		{
			lowerNode = m_Nodes[index].get();
		}
		// これで配列の範囲外にアクセスすることはなくなったため安全にアクセスできる
		// くっつけられるか判定
		if (newNode.get()->CanAttach(upperNode, lowerNode))
		{
			// 追加する
			if (index == -1 || index == static_cast<int>(m_Nodes.size()))
			{
				// 最後尾に追加
				m_Nodes.push_back(std::move(newNode));
				// タイムライン変更処理
				ModifyTimeLine();
				return static_cast<T*>(m_Nodes.back().get());
			}
			else
			{
				// 任意の位置に追加
				m_Nodes.insert(m_Nodes.begin() + index, std::move(newNode));
				// タイムライン変更処理
				ModifyTimeLine();
				return static_cast<T*>(m_Nodes[index].get());
			}
		}
		else
		{
			// 追加しない
			return nullptr;
		}
	};
private:
	inline static const Vector2 NODE_START = { 20.0f, 275.0f }; // ノードと文字の余白
	void ModifyTimeLine(); // タイムラインを修正する
	bool m_IsSelected = false; // 現在選択されているタブかどうか
	std::vector<std::unique_ptr<NodeBase>> m_Nodes; // 現在タブ内でくっついているノードのリスト
	//std::list<NodeBase> m_CanUseNodes; // タブ内で使用可能なノードのリスト(設置するための物、呼び出された際にプレイヤー側からソートしてここにいれる感じになるかな)
	int m_Index = 0; // タブのインデックス
	int m_CDMax = 0; // タブ内にあるノードをすべて合計したクールダウン
	std::list<int> m_NodeTimeLine; // タブ内にあるノードのcdが終わるタイミングを開始時から数えたときのリスト
	// あと上記変数のめんどいところはすでに使われているか否かをどうにかしないといけない
	// プレイヤー側に実態は持っておいて、ここはあくまでポインタとして持っておき、実態にフラグ変数を設けることで管理しやすい、、、みたいな構造が望ましい。
};