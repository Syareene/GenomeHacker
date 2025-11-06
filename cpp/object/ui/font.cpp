// 実際に使用する環境でのヘッダーは適宜追加してください
#include "main.h"
#include "lib/renderer.h"

#include "object/ui/font.h"

// DirectWrite描画クラス
DirectWriteCustomFont* Write;

int drawNum = 0;

void DirectWriteSample::Init(Transform trans)
{
	// DirectWrite用コンポーネントを作成
	Write = new DirectWriteCustomFont(&m_FontData);

	// 初期化
	Write->Init(Renderer::GetSwapChain());

	// フォントデータを改変
	m_FontData.fontSize = 60;
	m_FontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_FontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_FontData.font = Write->GetFontName(0);
	m_FontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_FontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

	// フォントをセット
	Write->SetFont(m_FontData);
}

void DirectWriteSample::Uninit()
{
	// メモリ解放
	delete Write;
	Write = nullptr;
}

void DirectWriteSample::Update()
{
}

void DirectWriteSample::Draw()
{
	Write->DrawString("ここはテスト画面です", Vector2(90, 90), D2D1_DRAW_TEXT_OPTIONS_NONE);

	Write->DrawString("描画位置は調整してください", Vector2(90, 390), D2D1_DRAW_TEXT_OPTIONS_NONE);
}