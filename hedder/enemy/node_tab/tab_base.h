#pragma once

#include "object/ui/button.h"
#include "player.h"
#include "enemy/node/base.h"
#include <vector>
#include <memory>

template<typename T>
concept NodeType = std::is_base_of_v<NodeBase, T>;

class TabBase
{
	// すべてのタブが継承するタブの基底クラス


	// 中身としては、
	// タブ部分の描画
	// 中に設置されているスクリプトのlist->中のリストに対して更新処理実行。
	// 背景の描画->dna_screen_scriptに情報を渡せば良い
	// タブの切り替え処理(button?)
	// んーこれbutton+αにしたほうが良さそげ、、

public:
	static constexpr size_t MAX_OBJECTS = 1; // オブジェクトvector最大数。継承先クラスで変更可能。
	virtual void Init(const unsigned int& playerId, Transform trans = Transform());
	//inline Player* GetPlayerPtr() { return m_PlayerPtr; } // プレイヤーのポインタを取得
	std::vector<NodeBase>& GetNodes() { return m_Nodes; } // 現在タブ内でくっついているノードのリストを取得
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

		// 挿入位置のイテレーターを取得
		auto it = (index == -1) ? m_Nodes.end() : m_Nodes.begin() + index;

		// ノードを直接構築
		NodeBase& newNode = *m_Nodes.emplace(it);
		
		// 初期化
		newNode.Init(trans);
		NodeBase* upperNode = nullptr;
		NodeBase* lowerNode = nullptr;

		// 挿入したノードの実際のインデックスを取得
		int actualIndex = std::distance(m_Nodes.begin(), m_Nodes.begin() + index);

		// 上側ノード挿入判定
		if (actualIndex > 0)
		{
			upperNode = &m_Nodes[actualIndex - 1];
		}
		// 下側ノード挿入判定
		if (actualIndex < static_cast<int>(m_Nodes.size()) - 1)
		{
			lowerNode = &m_Nodes[actualIndex + 1];
		}

		// くっつけられるか判定
		if (newNode.CanAttach(upperNode, lowerNode))
		{
			// タイムライン変更処理
			ModifyTimeLine();
			return static_cast<T*>(&newNode);
		}
		else
		{
			// 追加できない場合は、構築した要素を削除
			m_Nodes.erase(m_Nodes.begin() + actualIndex);
			return nullptr;
		}
	};
private:
	void ModifyTimeLine(); // タイムラインを修正する
	std::vector<NodeBase> m_Nodes; // 現在タブ内でくっついているノードのリスト
	int m_Index = 0; // タブのインデックス
	int m_CDMax = 0; // タブ内にあるノードをすべて合計したクールダウン
	std::list<int> m_NodeTimeLine; // タブ内にあるノードのcdが終わるタイミングを開始時から数えたときのリスト
	// あと上記変数のめんどいところはすでに使われているか否かをどうにかしないといけない
	// プレイヤー側に実態は持っておいて、ここはあくまでポインタとして持っておき、実態にフラグ変数を設けることで管理しやすい、、、みたいな構造が望ましい。
};