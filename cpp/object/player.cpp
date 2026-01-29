#include "main.h"
#include "lib/renderer.h"
#include "player.h"
#include "lib/modelRenderer.h"
#include "lib/input.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "enemy/bullet.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "collider/sphere.h"
#include "enemy/node/visual_base.h"
#include "enemy/node/move_x.h"
#include "enemy/node/move_z.h"

void Player::Init(Transform trans)
{
	// 画像が小さいため相対的に拡大する
	Transform player_trans;
	player_trans.SetScale(trans.GetScale().mul(Vector3(1.5f, 1.5f, 1.0f)));
	player_trans.SetPosition(trans.GetPosition() + Vector3(0.0f, player_trans.GetScale().y * 0.5f, 0.0f));

	SetTransform(player_trans);

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\hero.png"));

	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(GetScale().mul(Vector3(0.5f, 0.5f, 0.5f)));
	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform, Vector3(GetScale().x * 0.05f, 0.0f, 0.0f));

	// 生成するであろうBulletのmanager空間予約をする
	Manager::GetCurrentScene()->ReserveObject<Bullet>(Bullet::MAX_OBJECTS);


	m_HavingNodes.emplace_back(std::make_unique<MoveX>());
	m_HavingNodes.back()->Init(); // 初期化
	m_HavingNodes.back()->SetNodeLocation(NodeBase::NodeLocation::Player);
	m_HavingNodes.emplace_back(std::make_unique<MoveZ>());
	m_HavingNodes.back()->Init(); // 初期化
	m_HavingNodes.back()->SetNodeLocation(NodeBase::NodeLocation::Player);
	// プレイヤー側にもVisualを管理する配列があるが生成はDnaScreenScript側のinitで行う。


	AddTag("in_game");
	AddTag("player");
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
		SetRotation(Vector3(GetRotation().x, rotation.y + 180.0f, GetRotation().z)); // yもともとはXM_PI
	}

	if (Input::GetKeyPress('A'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * -0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y - 90.0f, GetRotation().z)); // yもともとは-XM_PI/2
	}

	if (Input::GetKeyPress('D'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * 0.1f));
		SetRotation(Vector3(GetRotation().x, rotation.y + 90.0f, GetRotation().z)); // yもともとはXM_PI/2
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
		bullet->SetVelocity(GetForward() * 0.15f);
	}

	if (Input::GetKeyTrigger('R'))
	{
		camera->Shake(Vector3(0.5f, 0.5f, 0.0f));
	}

}

void Player::Draw()
{
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

	ID3D11ShaderResourceView* texture = TextureManager::Get3DTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	// コリジョン描画(デバッグ用)
	if (!GetCollider())
	{
		return;
	}
	GetCollider()->DrawCollider();
}