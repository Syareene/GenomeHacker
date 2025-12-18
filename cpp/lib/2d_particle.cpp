#include "main.h"
#include "lib/2d_particle.h"
#include "lib/renderer.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "manager/texture_manager.h"
#include "manager/shader_manager.h"

void Particle2D::Register(int maxCount)
{
	// 結局これ汎用性出そうと思ってたけど一時変数でやってる時点で汎用的ではないかも
	// 最終的にはちゃんと格納してる場所からデータを引っ張ってこないとかな

	maxParticles = maxCount;
	m_Particles.resize(maxParticles);
	m_InstanceDataBuffer.resize(maxParticles);
	// デフォルトで全パーティクルを非アクティブに
	for (auto& p : m_Particles)
	{
		p.IsActive = false;
	}


	// ストラクチャードバッファ作成
	InstanceData* data = new InstanceData[maxParticles];
	// 初期値をセット
	for (int i = 0; i < maxParticles; i++)
	{
		data[i].PositionAndSize = XMFLOAT4(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100.0f, 0.0f);
		data[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		data[i].UVOffset = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	// shaderに渡したいプロパティをInstanceDataで渡す

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(InstanceData) * maxParticles;
	bd.StructureByteStride = sizeof(InstanceData);
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = data;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_InstanceBuffer);

	delete[] data;

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = maxParticles;

	// 設定
	Renderer::GetDevice()->CreateShaderResourceView(m_InstanceBuffer, &srvd, &m_InstanceSRV);
}


void Particle2D::Init(Transform trans)
{
	SetTransform(trans);

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\dna.png"));

	// シェーダー設定
	Renderer::CreatePixelShader(GetPixelShaderPointer(), "shader\\titleParticlePS.cso");
	Renderer::CreateVertexShader(GetVertexShaderPointer(), GetVertexLayoutPointer(), "shader\\titleParticleVS.cso");

	SetScale(Vector3(1.0f, 1.0f, 1.0f));
}

void Particle2D::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	m_InstanceBuffer->Release();
	m_InstanceSRV->Release();
	UninitDrawMember();
}

void Particle2D::Update()
{
	m_Count++;
	if (m_Count >= 20)
	{
		XMFLOAT4 color = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		// ランダムでRかGかBを1.0に
		int random = std::rand() % 3;
		if(random == 0)
		{
			color.x = 1.0f;
		}
		else if(random == 1)
		{
			color.y = 1.0f;
		}
		else
		{
			color.z = 1.0f;
		}

		Emit(1, Vector2(static_cast<float>(std::rand() % SCREEN_WIDTH), 0.0f), 50.0f, color);
		m_Count = 0;
	}
	UpdateParticle();
}

void Particle2D::UpdateParticle()
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
		p.LifeTime -= 1;
		if (p.LifeTime <= 0) 
		{
			p.IsActive = false;
			continue;
		}

		// 設定された更新ロジックを実行（ここがカスタマイズ可能）
		if (updateBehavior)
		{
			updateBehavior(p, 1);
		}

		// 一旦めんどいので固定値で動かす
		p.Position.y += 1.5f;


		// 物理挙動（共通部分）
		p.Position.x += p.Velocity.x * 1;
		p.Position.y += p.Velocity.y * 1;

		// GPU用データに変換してコピー
		gpuPtr[activeParticleCount].PositionAndSize = XMFLOAT4(p.Position.x, p.Position.y, p.Size, 0.0f);
		gpuPtr[activeParticleCount].Color = p.Color;
		gpuPtr[activeParticleCount].UVOffset = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		// UV計算が必要ならここで行う

		// ここで新たにm_Particlesのパーティクルがアクティブであるか確認
		if (p.IsActive) 
		{
			// インクリメント
			activeParticleCount++;
		}
	}

	if (activeParticleCount > 0)
	{
		// gpuのバッファ更新
		Renderer::GetDeviceContext()->UpdateSubresource(m_InstanceBuffer, 0, NULL, m_InstanceDataBuffer.data(), 0, 0);
	}
}

void Particle2D::Draw()
{
	// 描画できるパーティクルがないなら描画しない
	if(activeParticleCount <= 0)
	{
		return;
	}

	Renderer::SetDepthEnable(false);

	// 加算合成on
	//Renderer::GetDeviceContext()->OMSetBlendState(Renderer::GetBlendState(), NULL, 0xffffffff);


	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(GetVertexLayout());
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(GetPixelShader(), NULL, 0);

	Renderer::SetWorldViewProjection2D();

	// マテリアル設定
	SetMaterialOnDraw();

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

	// ストラクチャードバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_InstanceSRV);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// インスタンシングの描画はこっちでやろう
	Renderer::GetDeviceContext()->DrawInstanced(4, activeParticleCount, 0, 0);


	Renderer::SetDepthEnable(true);
	// 加算合成off(デフォに戻す)
	//Renderer::GetDeviceContext()->OMSetBlendState(Renderer::GetBlendStateAlpha(), NULL, 0xffffffff);
}