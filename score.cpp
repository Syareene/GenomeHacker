#include "score.h"
#include "shader_manager.h"


void Score::Init()
{
	// 頂点データ
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, GetVertexBufferPointer());

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\number.png"));

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);

	// 座標設定
	SetPosition(Vector3(SCREEN_WIDTH - 100.0f, 50.0f, 0.0f)); // 画面右上に配置
}

void Score::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void Score::Update()
{
	
}

void Score::Draw()
{

	Renderer::GetDeviceContext()->Unmap(GetVertexBuffer(), 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	SetMaterialOnDraw();

	for(int i = 0; i < ShowableDigits; i++)
	{
		// スコアの各桁を描画
		// 今の形式だとマトリクスも変えないとダメだ～～～wなのでサイズを下にposも動かす感じかなぁ。
		// 前のやつも結局都度頂点データ更新しているので、各種マトリックスの設定を呼びつつtexcoordだけvertexの設定を更新する?
		// んでmatrixに対して計算+サイズ分の値を入れて上げる感じで


		// 頂点バッファ設定
		SetVertexBufferOnDraw();
		// プロジェクションマトリックス設定
		SetProjectionMatrixOnDraw();
		// ビューマトリックス設定
		SetViewMatrixOnDraw();
		// 移動、回転マトリックス設定
		SetWorldMatrixOnDraw();
		
		// 頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float textureWidth = 1.0f / 5.0f; // テクスチャの横幅を4分割
		float textureHeight = 1.0f / 5.0f; // テクスチャの縦幅を4分割
		float offsetX = (m_Score % 4) * textureWidth; // フレームに応じたXオフセット
		float offsetY = (m_Score / 4) * textureHeight; // フレームに応じたYオフセット

		vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(offsetX, offsetY);

		vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(offsetX + textureWidth, offsetY);

		vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(offsetX, offsetY + textureHeight);

		vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(offsetX + textureWidth, offsetY + textureHeight);

		// 描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}