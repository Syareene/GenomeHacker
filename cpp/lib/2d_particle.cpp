#include "main.h"
#include "lib/2d_particle.h"
#include "lib/renderer.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "manager/texture_manager.h"
#include "manager/shader_manager.h"


void Particle2D::Init(Transform trans)
{
	SetTransform(trans);

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
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, GetVertexBufferPointer());


	// ストラクチャードバッファ作成
	Vector2* pos = new Vector2[maxParticles];
	for(int i = 0; i < maxParticles; ++i)
	{
		pos[i] = Vector2(0.0f, 0.0f);
	}

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vector3) * maxParticles;
	bd.StructureByteStride = sizeof(Vector3);
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = pos;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, GetVertexBufferPointer());

	delete[] pos;

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = 1000;

	HRESULT result = Renderer::GetDevice()->CreateShaderResourceView(GetVertexBuffer(), &srvd, &m_PositionSRV);



	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\particle.png"));

	// シェーダー設定
	Renderer::CreatePixelShader(GetPixelShaderPointer(), "shader\\instancingPS.cso");
	Renderer::CreateVertexShader(GetVertexShaderPointer(), GetVertexLayoutPointer(), "shader\\instancingVS.cso");

	for(int i = 0; i < maxParticles; ++i)
	{
		m_Particles[i].IsActive = false; // 初期化
	}
	SetScale(Vector3(0.3f, 0.3f, 0.3f)); // パーティクルのスケールを設定
}

void Particle2D::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
	m_PositionSRV->Release();
}

void Particle2D::Update()
{
	// updateparticleをどうにかして呼び出す必要あり
	UpdateParticle(Manager::m_DeltaTime);
}

void Particle2D::UpdateParticle(float deltaTime)
{
	activeParticleCount = 0;

	// インスタンスバッファの書き込みポインタ
	auto* gpuPtr = m_InstanceDataBuffer.data();

	for (auto& p : m_Particles) 
	{
		if (!p.IsActive)
		{
			continue;
		}

		// 寿命計算
		p.LifeTime -= deltaTime;
		if (p.LifeTime <= 0) 
		{
			p.IsActive = false;
			continue;
		}

		// ★設定された更新ロジックを実行（ここがカスタマイズ可能）
		if (updateBehavior)
		{
			updateBehavior(p, deltaTime);
		}

		// 物理挙動（共通部分）
		p.Position.x += p.Velocity.x * deltaTime;
		p.Position.y += p.Velocity.y * deltaTime;

		// GPU用データに変換してコピー
		gpuPtr[activeParticleCount].PositionAndSize = XMFLOAT4(p.Position.x, p.Position.y, p.Size, 0.0f);
		gpuPtr[activeParticleCount].Color = p.Color;
		// UV計算が必要ならここで行う

		// インクリメント
		activeParticleCount++;
	}
}

void Particle2D::Draw()
{
	Renderer::SetDepthEnable(false);

	Renderer::GetDeviceContext()->Unmap(GetVertexBuffer(), 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(GetVertexLayout());
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(GetPixelShader(), NULL, 0);

	// マテリアル設定
	SetMaterialOnDraw();

	// 頂点バッファ設定
	SetVertexBufferOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// ストラクチャードバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();
	//XMMATRIX projection, view;

	// ビューの逆行列(ビルボード描画のため)
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, camera->GetViewMatrix());
	invView.r[3].m128_f32[0] = 0.0f; // カメラの位置を無視
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f; // カメラの位置を無視

	// 描画
	for(int i = 0; i < maxParticles; ++i)
	{
		if (m_Particles[i].IsActive)
		{
			XMMATRIX trans, world, rot, scale;
			trans = XMMatrixTranslation(m_Particles[i].Position.x, m_Particles[i].Position.y, 0.0f);
			rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
			scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	// インスタンシングの描画はこっちでやろう
	Renderer::GetDeviceContext()->DrawInstanced(4, activeParticleCount, 0, 0);


	Renderer::SetDepthEnable(true);
}