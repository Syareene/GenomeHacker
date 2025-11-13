// 実際に使用する環境でのヘッダーは適宜追加してください
#include "main.h"
#include "lib/renderer.h"

#include "object/ui/font.h"

// DirectWrite描画クラス
//DirectWriteCustomFont* Write;

int drawNum = 0;

static int count = 0;

// ここ、常にフォント作成されてて解放されずにメモリ常に食い続ける仕様になってそう?

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
	// メモリ使用量が増加していないかを確認するためデバッグ用にここで毎度SetTextを呼んで確認
	SetDisplayText("これはサンプルテキストです" + std::to_string(count));
	count++;
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
}

void Font::GetDisplayText(std::string& outText) const
{
	outText = m_DisplayText;
}