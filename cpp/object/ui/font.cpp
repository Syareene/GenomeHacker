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
	m_Write->SetFont(m_FontData);
	SetDisplayText(text);

	// 位置セット
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
}

void Font::Init(Transform trans)
{
	// transformセット
	SetTransform(trans);

	// DirectWrite用コンポーネントを作成
	m_Write = std::make_unique<DirectWriteCustomFont>(&m_FontData);

	// 初期化
	m_Write->Init(Renderer::GetSwapChain());

	// フォントデータを改変
	m_FontData.fontSize = 60;
	m_FontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_FontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_FontData.font = m_Write->GetFontName(0);
	m_FontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_FontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_FontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	m_FontData.outlineWidth = 6.0f;

	// フォントをセット
	m_Write->SetFont(m_FontData);
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
	m_Write->DrawString(m_DisplayText, Vector2(GetPosition().x, GetPosition().y), D2D1_DRAW_TEXT_OPTIONS_NONE, false, true);

	//m_Write->DrawString("ここからいい感じにしたいね", Vector2(90, 680), D2D1_DRAW_TEXT_OPTIONS_NONE, false, true);
}

void Font::SetDisplayText(const std::string& text)
{
	m_DisplayText = text;
	// フォントキャッシュを更新
	m_Write->SetText(m_DisplayText);

	// --- 取得例: DIP とピクセルでサイズを取る ---
	FLOAT wDips = 0.0f, hDips = 0.0f;
	if (SUCCEEDED(m_Write->GetTextSizeDips(m_DisplayText, &wDips, &hDips)))
	{
		// 必要ならここで UI の矩形を更新したり当たり判定に使う
		// デバッグ出力（ワイド文字列に変換して表示）
		wchar_t buf[256];
		swprintf_s(buf, L"Text size (DIPs): w=%.2f h=%.2f\n", wDips, hDips);
		OutputDebugStringW(buf);
	}

	FLOAT wPx = 0.0f, hPx = 0.0f;
	if (SUCCEEDED(m_Write->GetTextSizePixels(m_DisplayText, &wPx, &hPx)))
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