#pragma once

//#include <list>
#include "object/2d_object.h"
#include "enemy/field_enemy.h"
#include "object/ui/font.h"

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

	struct NodeDescription
	{
		std::string desc;
		Vector2 text_pos;
	};
	
	// このノード内で追加でくっつけられるノード(数字系のノード等)
	// このとき、内部にあるノードが先に引っかかるようなコードを組まないとね

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	void MoveNodeToMouse();
	void FixFontPositions(Vector2 diff); // フォントの位置を説明文の位置に合わせて修正する
	virtual bool NodeEffect(FieldEnemy* enemy_ptr); // cd管理して終わったならtrueを返す
	// 更新処理(ノード持ったときにくっつけられるならくっつける等)->insertするみたいな処理がちょいめんどそうか。
	// ノードの処理効果
	const bool CanAttach(NodeBase* upper_node, NodeBase* lower_node) const;
	inline const int GetCDMax() const { return m_CDMax; }

	inline static const Vector2 NODE_MARGIN = { 10.0f, 10.0f }; // ノードと文字の余白
protected:
	// くっつけられるか判定関数
	inline void AddInputTypeTop(const InputType& type) { m_InputTypesTop.push_back(type); }
	inline void AddInputTypeBottom(const InputType& type) {m_InputTypesBottom.push_back(type);}
	inline const std::string& GetName() const { return m_Name; }
	inline void SetName(const std::string& name) { m_Name = name; }

	inline const virtual std::vector<NodeDescription*> GetDescriptions() const = 0;

	inline const virtual NodeDescription* GetDescription(const int index) const = 0;
	//inline const NodeDescription* GetDescription(const int index) const { return m_Description[index].get(); }

	inline virtual void AddDescription(const NodeDescription& desc) = 0;
	//inline void AddDescription(const NodeDescription& desc)
	//{
	//	std::unique_ptr<NodeDescription> desc_ptr = std::make_unique<NodeDescription>();
	//	m_Description.push_back(std::move(desc_ptr));
	//}

	inline virtual void SetDescriptionFontData(const FontData& fontData) = 0;
	//inline void SetDescriptionFontData(const FontData& fontData)
	//{
	//	m_DescFontData = fontData;
	//}

	inline virtual FontData& GetDescriptionFontData() = 0;

	// dna_editに行った時に表示するフォントオブジェクト郡(Fontの詳細な色とかはm_DescFontDataから引っ張る)
	inline virtual void AddFont(const std::string& text, const Vector2& pos) = 0;
	inline virtual const std::vector<Font*> GetFonts() = 0;

	inline const int GetID() const { return m_ID; }
	inline void SetID(const int id) { m_ID = id; }
	inline const std::string& GetKeyword() const { return m_Keyword; }
	inline void SetKeyword(const std::string& key) { m_Keyword = key; }
	inline void SetCDMax(const int cdMax) { m_CDMax = cdMax; }
	inline const int GetCD() const { return m_CD; }
	inline void SetCD(const int cd) { m_CD = cd; }
	inline void AddFontPtr(Font* fontPtr) { m_Fonts.push_back(fontPtr); }
	inline std::vector<Font*>& GetFontPtrs(std::vector<Font*>& outFonts) const
	{
		outFonts = m_Fonts;
	}
	inline Font& GetFontAt(const int index) const { return *(m_Fonts[index]); }
private:
	inline const std::vector<InputType>& GetInputTypesTop() const { return m_InputTypesTop; }
	inline const std::vector<InputType>& GetInputTypesBottom() const { return m_InputTypesBottom; }
	inline std::vector<std::unique_ptr<NodeBase>>& GetChildNodes() { return m_ChildNodes; }
	// ここの2つ、今のところサイズ3超えないからlistじゃなくてもいい説はある。
	std::vector<InputType> m_InputTypesTop; // くっつけられる形のリスト(上)
	std::vector<InputType> m_InputTypesBottom; // このノードに対してくっつけられる形(下)
	//std::list<NodeBase*> m_AttachedNodes; // くっつけられたノードのリスト->どの形が入るかを制限する必要がありそうだから既定クラスではなく派生クラスにするのはありかな
	// ないしは、ここで何も無い関数だけ作っておいてoverrideできるようにしておくとかね->内部だけで参照し完結する処理で作成。
	std::vector<std::unique_ptr<NodeBase>> m_ChildNodes; // 内部にくっつけられたノード群->unique_ptrで管理
	std::vector<Font*> m_Fonts; // 派生クラスで管理しているstaticなポインタをここに保存->フォント諸々参照用
	std::string m_Name; // ノードの名前(表示名、いらないかも)
	// ゲーム内に表示するテキストの文言->内部にある子ノードの位置を考慮して色々組まないといけないのだけがネック。	子ノード自体の位置はこの座標からの相対座標でいいんだけどね。
	//std::vector<std::unique_ptr<NodeDescription>> m_Description; // ノードの説明部分
	//std::vector<std::unique_ptr<Font>> m_DescriptionFonts; // dna_editに行った時に表示するフォントオブジェクト郡
	//FontData m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)
	int m_ID; // ノードのid(内部利用用)
	std::string m_Keyword; // ノードのキーワード
	int m_CDMax = 0; // ノードのクールダウン最大値(フレーム数)
	int m_CD = 0; // ノードのクールダウン(フレーム数)
};