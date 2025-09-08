#include "main.h"
#include "renderer.h"
#include "enemy/enemy.h"
#include "input.h"
#include "object/camera.h"
#include "manager.h"
#include "texture_manager.h"
#include "shader_manager.h"

// フィールドに居る敵のクラス。field_enemyに後々名前変えたほうがいいね。
// データ自体はenemyのbase_dataに格納。相手の行動をプログラムできる都合上元データを参考にして
// 挙動とかを動かす感じに。

// field_enemy側に処理を作成したのでここのプログラム、使わなくなります。



void Enemy::Init(Transform trans)
{
	SetTransform(trans);
	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	// シェーダー設定
	//Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);

	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\slime.png"));
	SetScale({ 2.0f, 2.0f, 1.0f });
	//SetPosition(GetPosition() + Vector3(0.0f, -1.0f, 0.0f));

	AddTag("InGame");
	AddTag("Enemy");
	
	// うーん、頂点変更可能にしたら描画できたなぁ。

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
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
}

void Enemy::Uninit()
{
	//m_VertexLayout->Release();
	//m_VertexShader->Release();
	//m_PixelShader->Release();
	TextureManager::UnloadTexture(GetTextureID());
}

void Enemy::Update()
{
	
}

void Enemy::Draw()
{
	// explosionから持ってっきたdraw部分(billboard)

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::NoAlphaVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::NoAlphaVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::NoAlphaPixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	SetWorldMatrixOnDrawBillboard();

	// マテリアル設定
	SetMaterialOnDraw();

	// 頂点バッファ設定
	SetDefaultVertexBufferBillboardOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);



	// old
	/*
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y + XM_PI, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);
	*/
}