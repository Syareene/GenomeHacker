#include "main.h"
#include "lib/renderer.h"
#include "enemy/enemy_bullet.h"
#include "lib/modelRenderer.h"
#include "lib/input.h"
#include "object/camera.h"
#include "scene/manager.h"
#include "player.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "collider/sphere.h"
#include "enemy/explosion.h"

void EnemyBullet::Init(Transform trans)
{
	// モデル読み込み（敵の弾用のモデルを使用、なければ同じものを使用）
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	AddTag("in_game");
	AddTag("enemy_bullet");

	SetTransform(trans);

	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(Vector3(0.225f, 0.225f, 0.225f));
	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform);
}

void EnemyBullet::Uninit()
{
	delete m_ModelRenderer;
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void EnemyBullet::Update()
{
	// 移動
	SetPosition(GetPosition() + m_Velocity);

	// 生存時間デクリメント
	m_LifeTime--;

	// コライダの場所更新
	GetCollider()->Update(GetPosition());

	// プレイヤーとの衝突判定（敵の弾はプレイヤーにのみ当たる）
	std::vector<Player*> players = GetCollider()->GetHitObjectsByType<Player>();

	for (auto& player : players)
	{
		// 爆発エフェクトを生成
		Manager::GetCurrentScene()->AddGameObject<Explosion>(1)->SetPosition(player->GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
		// プレイヤーに当たったら削除
		SetDestroy(true);
		// TODO: プレイヤーにダメージを与える処理をここに追加
		// player->TakeDamage(m_BulletDamage);
		break; // 一つのプレイヤーに当たったらループを抜ける
	}

	// 敵との衝突判定（同じ種類の敵には当たらない）
	std::vector<FieldEnemy*> enemies = GetCollider()->GetHitObjectsByType<FieldEnemy>();

	for (auto& enemy : enemies)
	{
		// 同じ敵種類の場合は当たり判定をスキップ
		if (enemy->GetEnemyBase() &&
			enemy->GetEnemyBase()->GetEnemyID() == m_OwnerEnemyID)
		{
			continue; // 同じ敵種類なので当たり判定をスキップ
		}

		// 異なる種類の敵に当たったら爆発エフェクトを生成
		Manager::GetCurrentScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
		// 敵に当たったら削除
		SetDestroy(true);
		// TODO: 敵にダメージを与える処理をここに追加
		// enemy->TakeDamage(m_BulletDamage);
		break; // 一つの敵に当たったらループを抜ける
	}

	// 生存時間が0になったら削除
	if (m_LifeTime <= 0)
	{
		SetDestroy(true);
	}
}

void EnemyBullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
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