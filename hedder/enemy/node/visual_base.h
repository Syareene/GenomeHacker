#pragma once

#include "object/2d_object.h"
#include "object/ui/font.h"
#include "enemy/node/base.h"


class NodeBase; // 前方宣言
class TabVisual; // 前方宣言

// nodeのedit時に使用する見た目用のクラス
// 基礎データはNodeBase側から引っ張ってくる形
class VisualBase : public Object2D
{
public:
	VisualBase() = default;
	virtual ~VisualBase() {}
	VisualBase(VisualBase&& Other) noexcept
		: Object2D(std::move(Other))
		, m_Font(std::move(Other.m_Font))
		, m_DescriptionFonts(std::move(Other.m_DescriptionFonts))
		, m_HoverTimer(Other.m_HoverTimer)
		, m_ScreenID(Other.m_ScreenID)
		, m_NodeBaseIndex(Other.m_NodeBaseIndex)
		, m_BaseNodePtr(Other.m_BaseNodePtr)
		, m_NodeLocation(Other.m_NodeLocation)
		, m_IsUpdated(Other.m_IsUpdated)
	{
		Other.m_BaseNodePtr = nullptr;
	}
	VisualBase& operator=(VisualBase&& Other) noexcept
	{
		if (this != &Other)
		{
			Object2D::operator=(std::move(Other));
			m_Font = std::move(Other.m_Font);
			m_DescriptionFonts = std::move(Other.m_DescriptionFonts);
			m_HoverTimer = Other.m_HoverTimer;
			m_ScreenID = Other.m_ScreenID;
			m_NodeBaseIndex = Other.m_NodeBaseIndex;
			m_BaseNodePtr = Other.m_BaseNodePtr;
			m_NodeLocation = Other.m_NodeLocation;
			m_IsUpdated = Other.m_IsUpdated;
			Other.m_BaseNodePtr = nullptr;
		}
		return *this;
	}

	void Init(const unsigned int& screen_id, int base_index, NodeBase* node);
	void UpdateVisual(NodeBase* node_ptr); // nodeのデータを引っ張ってきて更新する
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// 元々nodebaseにあったやつ
	inline static const Vector2 NODE_MARGIN = { 10.0f, 10.0f }; // ノードと文字の余白
	constexpr static const int SHOW_DESC_TIME = 45; // 説明文を表示するまでのホバー時間(フレーム数)


	void MoveNodeToMouse();
	void FixFontPositions(Vector2 diff); // フォントの位置を説明文の位置に合わせて修正する
	// dna_editに行った時に表示するフォントオブジェクト郡(Fontの詳細な色とかはm_DescFontDataから引っ張る)
	const FontData& GetFontDataFromTextType(const NodeBase::TextType& type) const;


	void SetNameFont(const NodeBase::NodeTextData& data)
	{
		// フォントを作成
		m_Font.Init(Transform());
		// FontDataはtext_typeを元にして引っ張ってくる

		m_Font.Register(data.text_pos, GetFontDataFromTextType(data.text_type), data.text);
	}
	inline const Font* GetNameFont() const
	{
		return &m_Font;
	}

	void AddDescFont(const NodeBase::NodeTextData& data)
	{
		// フォントを作成
		m_DescriptionFonts.push_back(Font());
		m_DescriptionFonts.back().Init(Transform());
		m_DescriptionFonts.back().Register(data.text_pos, GetFontDataFromTextType(data.text_type), data.text);
	}
	inline const std::vector<Font>* GetDescFonts() const
	{
		return &m_DescriptionFonts;
	}
	inline Font* GetDescFontAt(const int index) { return &m_DescriptionFonts[index]; }

	inline NodeBase* GetBaseNodePtr() const { return m_BaseNodePtr; }

	inline void SetNodeLocation(const NodeBase::NodeLocation loc) { m_NodeLocation = loc; }
	inline const NodeBase::NodeLocation GetNodeLocation() const { return m_NodeLocation; }
	inline const int GetNodeBaseIndex() const { return m_NodeBaseIndex; }

private:
	bool IsMatchActiveTab();

	Font m_Font;
	std::vector<Font> m_DescriptionFonts;
	inline bool IsShowDesc() const { return m_HoverTimer > SHOW_DESC_TIME; } // ホバーしてから60フレーム以上経っていたら説明文表示
	int m_HoverTimer = 0; // ホバーしている時間(フレーム数)

	unsigned int m_ScreenID = 0; // dna_screenのID
	int m_NodeBaseIndex = -1; // 変換する前のnodebaseのindex

	NodeBase* m_BaseNodePtr = nullptr; // 元となるノードのポインタ
	NodeBase::NodeLocation m_NodeLocation = NodeBase::NodeLocation::Enemy; // ノードの配置場所(敵orプレイヤー)

	bool m_IsUpdated = false;
};