#pragma once

//#include <list>
#include "object/2d_object.h"

// 多分ui周りのクラス継承?わからんけど
class NodeBase : public Object2D
{
	// 

public:
	// これ考えたけどsprite作るのがめんどくさいから形みたいな感じじゃなくて内部処理でくっつくくっつかないとかになるのかな？〇〇の場所で使えるノードみたいな感じで
	enum InputType
	{
		None, // くっつかない	
		//Square,
		//Triangle,
		//Circle,
		Move,
		Attack,
		Death,
	};
	
	// このノード内で追加でくっつけられるノード(数字系のノード等)
	// このとき、内部にあるノードが先に引っかかるようなコードを組まないとね

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	virtual void NodeEffect();
	// 更新処理(ノード持ったときにくっつけられるならくっつける等)->insertするみたいな処理がちょいめんどそうか。
	// ノードの処理効果
protected:
	// くっつけられるか判定関数
	bool CanAttach(InputType type) const;
private:
	std::list<InputType> m_InputTypesTop; // くっつけられる形のリスト(上)
	std::list<InputType> m_InputTypesBottom; // このノードに対してくっつけられる形(下)
	//std::list<NodeBase*> m_AttachedNodes; // くっつけられたノードのリスト->どの形が入るかを制限する必要がありそうだから既定クラスではなく派生クラスにするのはありかな
	// ないしは、ここで何も無い関数だけ作っておいてoverrideできるようにしておくとかね->内部だけで参照し完結する処理で作成。
	std::string m_Name; // ノードの名前(表示名)
	// ゲーム内に表示するテキストの文言->内部にある子ノードの位置を考慮して色々組まないといけないのだけがネック。	子ノード自体の位置はこの座標からの相対座標でいいんだけどね。
	std::string m_Description; // ノードの説明文
	int m_ID; // ノードのid(内部利用用)
	std::string m_Keyword; // ノードのキーワード
	// 
};