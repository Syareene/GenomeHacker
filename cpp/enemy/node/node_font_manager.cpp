#include "main.h"
#include "enemy/node/node_font_manager.h"

std::vector<FontData> NodeFontManager::m_FontDataMap;

void NodeFontManager::Init()
{
	// 各フォントタイプを生成し格納する

	// あらかじめ要素数分の領域を確保
	m_FontDataMap.resize(static_cast<size_t>(TextType::ElemLength));

	// 各要素で生成
	FontData fontData;

	// description
		// 初期値セット
	fontData.fontSize = 24;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 2.5f;

	//m_DescriptionFonts.push_back(std::make_unique<Font>());
	//m_DescriptionFonts.back()->Init(Transform());
	//m_DescriptionFonts.back()->Register(Vector2(10.0f, 300.0f), m_DescFontData, "MoveX: このノードがある敵は毎フレームn分だけX軸に対し移動します。");
}

void NodeFontManager::Draw(std::string text, const Vector2& pos, TextType type)
{
	// 指定されたタイプのフォントデータを取得
	// FontData* fontData = &m_FontDataMap[static_cast<int>(type)];
	// フォント描画処理
	// DrawString(text, pos, fontData);
}