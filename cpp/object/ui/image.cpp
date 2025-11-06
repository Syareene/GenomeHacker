#include "main.h"
#include "object/ui/image.h"
#include "manager/shader_manager.h"


void ImageDraw::Init(Transform trans)
{
	SetTransform(trans);
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	SetProjectionMatrixOnDraw();

	// 頂点バッファ設定
	SetDefaultVertexBufferOnDraw();
	// ビューマトリックス設定
	SetViewMatrixOnDraw();
	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();
	// マテリアル設定
	SetMaterialOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void ImageDraw::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void ImageDraw::Update()
{
	// 基本画像なので更新事項はない
}

void ImageDraw::Draw()
{
	// 1回だけ描画がonでないなら描画する
	// 1回だけ描画onならそもそもコンストラクタから描画したいね

	if(GetNoUpdate() || !IsActive()) 
	{
		return; // 更新しない、または非アクティブなら描画しない
	}
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	SetDefaultVertexBufferOnDraw();
	// プロジェクションマトリックス設定
	//SetProjectionMatrixOnDraw();
	// ビューマトリックス設定
	SetViewMatrixOnDraw();
	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();
	// マテリアル設定
	SetMaterialOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}