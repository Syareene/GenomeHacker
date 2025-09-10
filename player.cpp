#include "main.h"
#include "renderer.h"
#include "player.h"
#include "modelRenderer.h"
#include "input.h"
#include "object/camera.h"
#include "manager.h"
#include "enemy/bullet.h"
#include "shader_manager.h"
#include "collider/sphere.h"

void Player::Init(Transform trans)
{
	SetTransform(trans);

	// モデル読み込み
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\player.obj");

	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform, Vector3(0.0f, 1.0f, 0.0f));

	AddTag("InGame");
	AddTag("Player");
}

void Player::Uninit()
{
	delete m_ModelRenderer;
}

void Player::Update()
{
	// vector3が間違ってる可能性とかもある?

	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();
	Vector3 rotation = camera->GetRotation();

	// キーで移動
	if (Input::GetKeyPress('W'))
	{
		SetPosition(GetPosition() + (camera->GetForward() * 0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y, GetRotation().z));
	}
	
	if (Input::GetKeyPress('S'))
	{
		SetPosition(GetPosition() + (camera->GetForward() * -0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y + XM_PI, GetRotation().z));
	}

	if (Input::GetKeyPress('A'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * -0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y - XM_PIDIV2, GetRotation().z));
	}

	if (Input::GetKeyPress('D'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * 0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y + XM_PIDIV2, GetRotation().z));
	}

	// コライダの場所更新(これ自動更新になるように変えたいね～～)
	GetCollider()->Update(GetPosition());

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		Transform trans = Transform();
		trans.SetPosition(GetPosition() + Vector3(0.0f, 1.0f ,0.0f));

		// 弾発射
		Bullet* bullet = Manager::GetCurrentScene()->AddGameObject<Bullet>(0, trans);
		// 初期座標設定v
		//bullet->SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
		// 弾の速度を設定
		bullet->SetVelocity(GetForward() * 0.2f);
	}
}

void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	//rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	rot = XMMatrixRotationRollPitchYaw(GetRadian().x, GetRadian().y + XM_PI, GetRadian().z);
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