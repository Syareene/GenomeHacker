#include "object/ui/image.h"


void ImageDraw::Init()
{
	
}

void ImageDraw::Uninit()
{
	// 画像終了処理
	TextureManager::UnloadTexture(GetTextureID());
}

void ImageDraw::Update()
{
	// 基本画像なので更新事項はない
}

void ImageDraw::Draw()
{
	// 1回だけ描画がonでないなら描画する
	// 1回だけ描画onならそもそもコンストラクタから描画したいね

	// 入力レイアウト設定
	//Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	// シェーダー設定
	//Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	// 移動、回転マトリックス設定
	//XMMATRIX
}