#include "main.h"
#include "renderer.h"
#include "object/polygon.h"
#include "manager.h"
#include "textureManager.h"
#include "shader_manager.h"


void Polygon2D::Init()
{
	SetDefaultVertex();

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\grass.jpg"));

	SetScale(Vector3(1.0f, 1.0f, 1.0f)); // スケールを設定
}

void Polygon2D::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void Polygon2D::Update()
{
	
}

void Polygon2D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	SetVertexBufferOnDraw();
	// プロジェクションマトリックス設定
	SetProjectionMatrixOnDraw();
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