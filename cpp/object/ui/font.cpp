// 実際に使用する環境でのヘッダーは適宜追加してください
#include "main.h"
#include "lib/renderer.h"
#include "object/ui/font.h"

// DirectWrite描画クラス
//DirectWriteCustomFont* Write;

void Font::Register(const Vector2& pos, const FontData& font_data, std::string text)
{
	// フォントデータを保存
	m_FontData = font_data;
	// テキストデータセット
	m_DisplayText = text;

	// 位置セット
	SetPosition(Vector3(pos.x, pos.y, 0.0f));

	// プレロード
	DirectWriteCustomFont::GetInstance()->PreCacheTextLayout(m_FontData, m_DisplayText);
	// width/height取得
	FLOAT wPx = 0.0f, hPx = 0.0f;
	if (SUCCEEDED(DirectWriteCustomFont::GetInstance()->GetTextSizePixels(m_DisplayText, &wPx, &hPx)))
	{
		wchar_t buf2[256];
		swprintf_s(buf2, L"Text size (px): w=%.1f h=%.1f\n", wPx, hPx);
		OutputDebugStringW(buf2);
		m_WidthHeight = Vector2(wPx, hPx);
	}
}

void Font::Init(Transform trans)
{
	// transformセット
	SetTransform(trans);

	// デフォルトフォントデータ設定
	m_FontData.fontSize = 60;
	m_FontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_FontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_FontData.font = DirectWriteCustomFont::GetInstance()->GetFontName(0);
	m_FontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_FontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_FontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	m_FontData.outlineWidth = 6.0f;
}

void Font::Uninit()
{
	
}

void Font::Update()
{
	//SetDisplayText("これはサンプルテキストです" + std::to_string(count));
}

void Font::Draw()
{
	// フォントのプリセットIDを取得
	int presetID = DirectWriteCustomFont::GetInstance()->FindOrCreateVisualPreset(m_FontData);
	
	DirectWriteCustomFont::GetInstance()->DrawString(m_DisplayText, presetID, Vector2(GetPosition().x, GetPosition().y), D2D1_DRAW_TEXT_OPTIONS_NONE, false, true);

	//m_Write->DrawString("ここからいい感じにしたいね", Vector2(90, 680), D2D1_DRAW_TEXT_OPTIONS_NONE, false, true);
	return;
}

void Font::SetDisplayText(const std::string& text)
{
	m_DisplayText = text;
	// フォントキャッシュを更新
	DirectWriteCustomFont::GetInstance()->PreCacheTextLayout(m_FontData, m_DisplayText);

	// これGetTextSizeDipsとかはキャッシュにある前提なのでDrawString呼んだ後じゃないと見れないな

	// テキストサイズ取得
	FLOAT wPx = 0.0f, hPx = 0.0f;
	if (SUCCEEDED(DirectWriteCustomFont::GetInstance()->GetTextSizePixels(m_DisplayText, &wPx, &hPx)))
	{
		wchar_t buf2[256];
		swprintf_s(buf2, L"Text size (px): w=%.1f h=%.1f\n", wPx, hPx);
		OutputDebugStringW(buf2);
		m_WidthHeight = Vector2(wPx, hPx);
	}
}

void Font::GetDisplayText(std::string& outText) const
{
	outText = m_DisplayText;
}