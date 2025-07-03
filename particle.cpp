#include "particle.h"

#include "main.h"
#include "renderer.h"
#include "object/camera.h"
#include "manager.h"
#include "textureManager.h"
#include "shader_manager.h"


void Particle::Init()
{
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

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\particle.png"));

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);

	for(int i = 0; i < MaxParticles; ++i)
	{
		m_Particles[i].Enable = false; // 初期化
	}
	SetScale(Vector3(0.3f, 0.3f, 0.3f)); // パーティクルのスケールを設定
}

void Particle::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void Particle::Update()
{
	for(int i = 0; i < MaxParticles; i++)
	{
		if (m_Particles[i].Enable == false)
		{
			m_Particles[i].Enable = true; // パーティクルを有効化
			m_Particles[i].LifeTime = 60; // 寿命を設定
			m_Particles[i].Position = GetPosition(); // パーティクルの位置を設定
			m_Particles[i].Velocity = Vector3(
				(static_cast<float>(rand() % 100 - 50) / 1000.0f), // -1.0f ~ 1.0f のランダム値
				(static_cast<float>(rand() % 100 + 50) / 1000.0f),
				(static_cast<float>(rand() % 100 - 50) / 1000.0f)
			); // ランダムな速度を設定
			break; // 新しいパーティクルを生成したらループを抜ける
		}

		for (int i = 0; i < MaxParticles; i++)
		{
			if (m_Particles[i].Enable)
			{
				// 動かす
				m_Particles[i].Velocity += Vector3(0.0f, -0.005f, 0.0f); // 重力の影響を受ける
				m_Particles[i].Position += m_Particles[i].Velocity; // 位置を更新
				m_Particles[i].LifeTime--; // 寿命を減らす
				if (m_Particles[i].LifeTime <= 0)
				{
					m_Particles[i].Enable = false; // 寿命が尽きたら無効化
				}
			}
		}
	}
}

void Particle::Draw()
{
	Renderer::SetDepthEnable(false);

	Renderer::GetDeviceContext()->Unmap(GetVertexBuffer(), 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);

	// マテリアル設定
	SetMaterialOnDraw();

	// 頂点バッファ設定
	SetVertexBufferOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

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
	for(int i = 0; i < MaxParticles; ++i)
	{
		if (m_Particles[i].Enable)
		{
			XMMATRIX trans, world, rot, scale;
			trans = XMMatrixTranslation(m_Particles[i].Position.x, m_Particles[i].Position.y, m_Particles[i].Position.z);
			rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
			scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
	Renderer::SetDepthEnable(true);
}