#include "main.h"
#include "lib/renderer.h"
#include "object/area_object.h"
#include "manager/texture_manager.h"
#include "manager/shader_manager.h"


void AreaObject::SetPipelineState()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::InstancingVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::InstancingVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::InstancingPixelShader, NULL, 0);
}

void AreaObject::UpdateGPUData(InstanceBufferData& data)
{
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	rot = XMMatrixRotationRollPitchYaw(GetRadian().x, GetRadian().y, GetRadian().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 結果をdataに格納
	XMStoreFloat4x4(&data.WorldMatrix, XMMatrixTranspose(world));
	// 色設定
	data.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// uv設定->ここ元頂点データちゃんと見てくれるから元々の頂点データのTexCoordがちゃんとuvテクスチャ用の座標になってればおけ
	data.UVOffset = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AreaObject::Init(Transform trans)
{
	SetTransform(trans);

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, 0.0f, 0.5f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, 0.0f, 0.5f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, 0.0f, -0.5f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.0f, -0.5f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, GetVertexBufferPointer());

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\area.png"));

	AddTag("in_game");
}

void AreaObject::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void AreaObject::Update()
{
	// カウントインクリメント
	m_Duration++;
	// もし持続時間が最大値を超えたら削除
	if (m_Duration >= m_MaxDuration)
	{
		SetDestroy(true);
	}
}
// gpuのやつとか作成する必要あり

void AreaObject::Draw()
{
	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();

	// マテリアル設定
	SetMaterialOnDraw();

	// 頂点バッファ設定
	SetVertexBufferOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::Get3DTexture(GetTextureID());
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}