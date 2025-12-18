// 基礎データからデータを持ってきてフィールドに描画する
// データを読み取ってから書くのでこのクラスは継承されずデータを受け取り挙動を変えるクラスとなる
#include "main.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "collider/sphere.h"
#include "player.h"

void FieldEnemy::Init(Transform trans)
{
	// 初期座標で設定する
	SetPosition(trans.GetPosition());
	SetRotation(trans.GetRotation());
	SetScale(trans.GetScale());

	// 初期化処理
	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(Vector3(0.30f, 0.30f, 0.30f));

	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform);

	//Object3D::Init();
	// テクスチャは敵データから描画時に取得するのでいらない
}

void FieldEnemy::Uninit()
{
	// 終了処理
	Object3D::Uninit();
}

void FieldEnemy::Update()
{
	// 判定リセット
	m_IsHit = false;

	// 体力が0以下かチェック
	if (m_CurrentHP <= 0.0f)
	{
		// 死亡ノードを実行
		bool isDead = m_EnemyBase->ExecuteDeath(this);
		if (isDead)
		{
			// trueが帰ってきたら自身を削除
			SetDestory(true);
		}
		// 0以下ならそれ移行の処理は実施したくないのでここでreturn
		return;
	}

	// 更新処理
	Object3D::Update();
	
	// 各敵のnodeを実行。
	m_EnemyBase->ExecuteAttack(this);
	m_EnemyBase->ExecuteMove(this);

	// コライダ更新
	GetCollider()->Update(GetPosition());

	// 当たってるコライダがあるかチェック
	std::list<Player*> p_hit = GetCollider()->GetHitObjectsByType<Player>();
	if (!p_hit.empty())
	{
		m_IsHit = true;
	}

	std::list<FieldEnemy*> e_hit = GetCollider()->GetHitObjectsByType<FieldEnemy>();
	for (auto& h : e_hit)
	{
		// 自分自身はスルー
		if(h == this)
		{
			continue;
		}
		m_IsHit = true;
	}
}

void FieldEnemy::Draw()
{
	// 描画処理
	//Object3D::Draw();
	
	// 敵データからテクスチャを取得して描画(テンプレ関数用意してないので一旦使い回しでやる)

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

	ID3D11ShaderResourceView* texture = TextureManager::Get3DTexture(GetEnemyBase()->GetEnemyTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	// コリジョン描画(デバッグ用)
	if(!GetCollider())
	{
		return;
	}
	GetCollider()->DrawCollider();
}