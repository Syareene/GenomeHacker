#pragma once

#include "enemy/field_enemy.h"
#include "object/ui/font.h"

class NodeBase
{
	// 

public:
	// ノードの入力タイプ
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

	enum class NodeLocation
	{
		Enemy,
		Player,
	};

	enum class TextType
	{
		Normal,
		Rare,
	};

	struct NodeTextData
	{
		std::string text;
		Vector2 text_pos;
		NodeBase::TextType text_type = NodeBase::TextType::Normal; // ここ参照してテンプレートとして使用
	};
	NodeBase() = default;
	virtual ~NodeBase() = default;
	NodeBase(NodeBase&&) noexcept = default; // ムーブコンストラクタ
	NodeBase& operator=(NodeBase&&) noexcept = default; // ムーブ代入演算子

	// これで保存、説明文も一旦単一に変更して複数対応する時にだけvectorに保存しつつ中心posを別で保存、この中身は相対posに変更という形で。
	// text_typeを元にFontData返す関数欲しいな

	FontData& GetFontDataFromTextType(const TextType& type) const;


	// このノード内で追加でくっつけられるノード(数字系のノード等)
	// このとき、内部にあるノードが先に引っかかるようなコードを組まないとね

	virtual void Init(Transform trans = Transform());
	virtual bool NodeEffect(FieldEnemy* enemy_ptr); // cd管理して終わったならtrueを返す
	// ノードの処理効果
	const bool CanAttach(NodeBase* upper_node, NodeBase* lower_node) const;
	inline const int GetCDMax() const { return m_CDMax; }
	inline const NodeLocation GetNodeLocation() const { return m_NodeLocation; }
	inline void SetNodeLocation(const NodeLocation loc) { m_NodeLocation = loc; }

	void UpdateDescriptionData();

	// ここあくまでデータをセットしただけで実際のfontobjは生成されていない!
	inline const NodeTextData& GetNameData() const { return m_Name; }
	inline void SetNameData(const NodeTextData& data) 
	{ 
		m_Name = data;
	}
	inline const NodeTextData& GetDescriptionData() const { return m_Description; };
	inline const int GetMoveManageId() const { return m_MoveManageId; }
	inline void SetMoveManageId(const int id) { m_MoveManageId = id; }
protected:
	// くっつけられるか判定関数
	inline void AddInputTypeTop(const InputType& type) { m_InputTypesTop.push_back(type); }
	inline void AddInputTypeBottom(const InputType& type) { m_InputTypesBottom.push_back(type); }

	// 説明文系はこっちに変える
	inline void SetDescriptionData(const NodeTextData& data)
	{
		// セット
		m_Description = data;
	}

	// ノードごとの説明文設定関数(override後この中でSetDescriptionDataを呼ぶ必要あり)
	virtual std::string GenerateDescriptionText()
	{
		assert(false && "GenerateDescriptionText not overridden");
		return "";
	}

	inline const int GetID() const { return m_ID; }
	inline void SetID(const int id) { m_ID = id; }
	inline const std::string& GetKeyword() const { return m_Keyword; }
	inline void SetKeyword(const std::string& key) { m_Keyword = key; }
	inline void SetCDMax(const int cdMax) { m_CDMax = cdMax; }
	inline const int GetCD() const { return m_CD; }
	inline void SetCD(const int cd) { m_CD = cd; }
private:
	inline const std::vector<InputType>& GetInputTypesTop() const { return m_InputTypesTop; }
	inline const std::vector<InputType>& GetInputTypesBottom() const { return m_InputTypesBottom; }
	inline std::vector<std::unique_ptr<NodeBase>>& GetChildNodes() { return m_ChildNodes; }
	// ここの2つ、今のところサイズ3超えないからlistじゃなくてもいい説はある。
	std::vector<InputType> m_InputTypesTop; // くっつけられる形のリスト(上)
	std::vector<InputType> m_InputTypesBottom; // このノードに対してくっつけられる形(下)
	std::vector<std::unique_ptr<NodeBase>> m_ChildNodes; // 内部にくっつけられたノード群->unique_ptrで管理
	NodeTextData m_Name; // ノードの名前(表示名、いらないかも)
	NodeTextData m_Description; // ノードの説明文群
	Font m_NameFont;
	std::vector<Font> m_DescriptionFonts;


	// ゲーム内に表示するテキストの文言->内部にある子ノードの位置を考慮して色々組まないといけないのだけがネック。	子ノード自体の位置はこの座標からの相対座標でいいんだけどね。
	NodeLocation m_NodeLocation = NodeLocation::Enemy; // ノードの設置場所(敵用かプレイヤー用か)
	int m_MoveManageId = 0; // VisualBase->NodeBase変換時に使用する割り振り用id変数
	int m_ID; // ノードのid(内部利用用)
	std::string m_Keyword; // ノードのキーワード
	int m_CDMax = 0; // ノードのクールダウン最大値(フレーム数)
	int m_CD = 0; // ノードのクールダウン(フレーム数)
	bool m_IsUpdated = true; // ノードが追加されたり変更されたものかどうか(visual_baseの再生成時にこれがtrueのものだけ再生成すれば良いかなといった感じ)
};