#include "main.h"
#include "lib/renderer.h"
#include "player.h"
#include "lib/modelRenderer.h"
#include "lib/input.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "enemy/bullet.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "manager/default_vertex.h"
#include "collider/sphere.h"
#include "enemy/node/visual_base.h"
// ノード系
#include "enemy/node/move_x.h"
#include "enemy/node/move_z.h"
#include "enemy/node/move_to_player.h"
#include "enemy/node/eight_shot.h"
#include "enemy/node/area.h"

void Player::SetPipelineState()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::InstancingVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::InstancingVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::InstancingPixelShader, NULL, 0);
}

void Player::UpdateGPUData(InstanceBufferData& data)
{
	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();

	// ビューの逆行列作成
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, camera->GetViewMatrix());
	invView.r[3].m128_f32[0] = 0.0f; // カメラの位置を無視
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f; // カメラの位置を無視

	XMMATRIX trans, world, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * invView * trans;

	// 結果をdataに格納
	XMStoreFloat4x4(&data.WorldMatrix, XMMatrixTranspose(world));
	// 色設定
	data.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// uv設定->ここ元頂点データちゃんと見てくれるから元々の頂点データのTexCoordがちゃんとuvテクスチャ用の座標になってればおけ
	data.UVOffset = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

void Player::Init(Transform trans)
{
	// 画像が小さいため相対的に拡大する
	Transform player_trans;
	player_trans.SetScale(trans.GetScale().mul(MODEL_SCALE));
	player_trans.SetPosition(trans.GetPosition() + Vector3(0.0f, player_trans.GetScale().y * 0.5f, 0.0f));

	SetTransform(player_trans);

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\hero.png"));

	// 頂点代入
	SetVertexBuffer(DefaultVertex::GetBillboardBuffer());

	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(GetScale().mul(COLLIDER_SCALE));
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
	m_HavingNodes.emplace_back(std::make_unique<MoveToPlayer>());
	m_HavingNodes.back()->Init(); // 初期化
	m_HavingNodes.back()->SetNodeLocation(NodeBase::NodeLocation::Player);
	m_HavingNodes.emplace_back(std::make_unique<EightShot>());
	m_HavingNodes.back()->Init(); // 初期化
	m_HavingNodes.back()->SetNodeLocation(NodeBase::NodeLocation::Player);
	m_HavingNodes.emplace_back(std::make_unique<Area>());
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
		SetPosition(GetPosition() + (camera->GetForward() * PLAYER_MOVE_SPEED));
		SetRotation(Vector3(GetRotation().x, rotation.y, GetRotation().z));
	}
	
	if (Input::GetKeyPress('S'))
	{
		SetPosition(GetPosition() + (camera->GetForward() * -PLAYER_MOVE_SPEED));
		SetRotation(Vector3(GetRotation().x, rotation.y + 180.0f, GetRotation().z)); // yもともとはXM_PI
	}

	if (Input::GetKeyPress('A'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * -PLAYER_MOVE_SPEED));
		SetRotation(Vector3(GetRotation().x, rotation.y - 90.0f, GetRotation().z)); // yもともとは-XM_PI/2
	}

	if (Input::GetKeyPress('D'))
	{
		SetPosition(GetPosition() + (camera->GetRight() * PLAYER_MOVE_SPEED));
		SetRotation(Vector3(GetRotation().x, rotation.y + 90.0f, GetRotation().z)); // yもともとはXM_PI/2
	}

	// コライダの場所更新(これ自動更新になるように変えたいね～～)
	GetCollider()->Update(GetPosition());

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		Transform trans = Transform();
		trans.SetPosition(GetPosition() + Vector3(0.0f, BULLET_SPAWN_OFFSET_Y ,0.0f));

		// 弾発射
		Bullet* bullet = Manager::GetCurrentScene()->AddGameObject<Bullet>(0, trans);
		// 初期座標設定v
		//bullet->SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
		// 弾の速度を設定
		bullet->SetVelocity(Manager::GetCurrentScene()->GetGameObject<Camera>()->GetForward() * BULLET_MOVE_SPEED);
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

	//// コリジョン描画(デバッグ用)
	//if (!GetCollider())
	//{
	//	return;
	//}
	//GetCollider()->DrawCollider();
}