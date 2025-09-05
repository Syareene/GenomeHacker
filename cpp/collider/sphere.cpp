#include "collider/sphere.h"
#include "main.h"
#include "renderer.h"
#include "shader_manager.h"

ID3D11Buffer* Sphere::m_VertexBuffer = nullptr;

void Sphere::Init()
{
	if(m_VertexBuffer)
	{
		// 既に生成されている
		return;
	}
	// 頂点バッファ生成
	// 頂点は12頂点で円を描く
	std::vector<Vector3> circleVertex;
	MakeCircleVertex(m_CircleVertexCount, circleVertex);
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vector3) * static_cast<UINT>(m_CircleVertexCount + 1); // (+1は円閉じるための描画用)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = circleVertex.data();
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

// これ開放するタイミング考えないとエラー出るわよ
void Sphere::Uninit()
{
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = nullptr;
	}
}

void Sphere::Update()
{
	// 特に何もしない
}


void Sphere::DrawCollider()
{
	// デバッグ時のみコライダを描画
#ifdef _DEBUG
	// 生成した頂点を使用し描画

	// シェーダーは一旦セットしない。後々デバッグ用に色を変えるシェーダー作成する。

	// 入力レイアウト設定(シェーダーのレイアウトなのでこれもシェーダー使用するときに用意)
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::DebugVertexLayout);
	// シェーダー設定->とりあえずunlitだがデバッグ用のを後々用意。
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::DebugVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::DebugPixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetCenter().x, GetCenter().y, GetCenter().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// マテリアル設定
	MATERIAL material;
	material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	// specとemissionもあるが一旦放置
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	// 頂点バッファ設定
	UINT stride = sizeof(Vector3);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	//ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	//Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 描画(+1は円閉じるための描画用)
	Renderer::GetDeviceContext()->Draw(m_CircleVertexCount + 1, 0);

	// 縦には生成できたので回転し横向きにも描画
	
	// 移動、回転マトリックス設定
	trans = XMMatrixTranslation(GetCenter().x, GetCenter().y, GetCenter().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x - 89.5f, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// 他は再利用するのでそのまま描画(+1は円閉じるための描画用)
	Renderer::GetDeviceContext()->Draw(m_CircleVertexCount + 1, 0);
#endif
}

void Sphere::MakeCircleVertex(int vertex_count, std::vector<Vector3>& outVertex)
{
	// Sphereの可視化
	// デバッグ用に球を描画する

	for (int i = 0; i <= vertex_count; i++)
	{
		Vector3 vertex;

		float theta = float(i) / float(vertex_count) * 2.0f * XM_PI;
		float x = GetRadius() * cosf(theta);
		float y = GetRadius() * sinf(theta);

		outVertex.push_back(Vector3(x, y, 0.0f));
	}

	// 最後に最初の頂点を追加して円を閉じる
	outVertex.push_back(outVertex[0]);
}