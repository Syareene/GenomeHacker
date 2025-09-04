#include "main.h"
#include "renderer.h"
#include "enemy/bullet.h"
#include "modelRenderer.h"
#include "input.h"
#include "object/camera.h"
#include "manager.h"
#include "enemy/enemy.h"
#include "enemy/explosion.h"

void Bullet::Init()
{
	// モデル読み込み
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddTag("InGame");
	AddTag("Bullet");
}

void Bullet::Uninit()
{
	delete m_ModelRenderer;
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bullet::Update()
{
	// vector3が間違ってる可能性とかもある?

	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();
	Vector3 rotation = camera->GetRotation();
	// 移動
	SetPosition(GetPosition() + m_Velocity);

	// 生存時間デクリメント
	//m_LifeTime--;

	if (GetPosition().length() > 25.0f)
	{
		// 画面外に出たら削除
		SetDestory(true);
	}

	// 敵との衝突判定
	auto enemies = Manager::GetCurrentScene()->GetGameObjects<Enemy>();

	for (auto& enemy : enemies)
	{
		Vector3 d = enemy->GetPosition() - GetPosition();
		float length = d.length();
		if (length < 1.0f)
		{
			// 爆発エフェクトを生成
			Manager::GetCurrentScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 1.5f, 0.0f));

			// 敵に当たったら削除
			SetDestory(true);
			enemy->SetDestory(true); // 敵も削除
		}
	}
}

void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	//rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y + XM_PI, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}