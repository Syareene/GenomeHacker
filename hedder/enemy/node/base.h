#pragma once

//#include <list>
#include "object/2d_object.h"
#include "enemy/field_enemy.h"

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

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	virtual bool NodeEffect(FieldEnemy* enemy_ptr); // cd管理して終わったならtrueを返す
	// 更新処理(ノード持ったときにくっつけられるならくっつける等)->insertするみたいな処理がちょいめんどそうか。
	// ノードの処理効果
	const bool CanAttach(NodeBase* upper_node, NodeBase* lower_node) const;
protected:
	// くっつけられるか判定関数
	inline void AddInputTypeTop(const InputType& type) { m_InputTypesTop.push_back(type); }
	inline void AddInputTypeBottom(const InputType& type) {m_InputTypesBottom.push_back(type);}
	inline const std::string& GetName() const { return m_Name; }
	inline void SetName(const std::string& name) { m_Name = name; }
	inline const std::string& GetDescription() const { return m_Description; }
	inline void SetDescription(const std::string& desc) { m_Description = desc; }
	inline const int GetID() const { return m_ID; }
	inline void SetID(const int id) { m_ID = id; }
	inline const std::string& GetKeyword() const { return m_Keyword; }
	inline void SetKeyword(const std::string& key) { m_Keyword = key; }
	inline const int GetCD() const { return m_CD; }
	inline void SetCD(const int cd) { m_CD = cd; }
private:
	inline const std::vector<InputType>& GetInputTypesTop() const { return m_InputTypesTop; }
	inline const std::vector<InputType>& GetInputTypesBottom() const { return m_InputTypesBottom; }
	// ここの2つ、今のところサイズ3超えないからlistじゃなくてもいい説はある。
	std::vector<InputType> m_InputTypesTop; // くっつけられる形のリスト(上)
	std::vector<InputType> m_InputTypesBottom; // このノードに対してくっつけられる形(下)
	//std::list<NodeBase*> m_AttachedNodes; // くっつけられたノードのリスト->どの形が入るかを制限する必要がありそうだから既定クラスではなく派生クラスにするのはありかな
	// ないしは、ここで何も無い関数だけ作っておいてoverrideできるようにしておくとかね->内部だけで参照し完結する処理で作成。
	std::string m_Name; // ノードの名前(表示名)
	// ゲーム内に表示するテキストの文言->内部にある子ノードの位置を考慮して色々組まないといけないのだけがネック。	子ノード自体の位置はこの座標からの相対座標でいいんだけどね。
	std::string m_Description; // ノードの説明文
	int m_ID; // ノードのid(内部利用用)
	std::string m_Keyword; // ノードのキーワード
	int m_CD = 0; // ノードのクールダウン(フレーム数)
};