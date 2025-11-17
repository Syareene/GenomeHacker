#include "main.h"
#include "lib/renderer.h"
#include "enemy/bullet.h"
#include "lib/modelRenderer.h"
#include "lib/input.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "enemy/field_enemy.h"
#include "collider/sphere.h"
#include "enemy/explosion.h"

void Bullet::Init(Transform trans)
{
	// モデル読み込み
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddTag("in_game");
	AddTag("bullet");

	SetTransform(trans);

	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(Vector3(0.225f, 0.225f, 0.225f));
	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform);
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
	m_LifeTime--;

	// コライダの場所更新
	GetCollider()->Update(GetPosition());

	// 敵との衝突判定
	std::list<FieldEnemy*> enemies = GetCollider()->GetHitObjectsByType<FieldEnemy>();

	for(auto& enemy : enemies)
	{
		// 爆発エフェクトを生成
		Manager::GetCurrentScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
		// 敵に当たったら削除
		SetDestory(true);
		enemy->SetCurrentHP(enemy->GetCurrentHP() - 1); // 敵の体力を減らす
		break; // 一つの敵に当たったらループを抜ける
	}

	// 生存時間が0になったら削除
	if(m_LifeTime <= 0)
	{
		SetDestory(true);
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

	// コリジョン描画(デバッグ用)
	if (!GetCollider())
	{
		return;
	}
	GetCollider()->DrawCollider();
}