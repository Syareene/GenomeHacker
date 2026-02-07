// 基礎データからデータを持ってきてフィールドに描画する
// データを読み取ってから書くのでこのクラスは継承されずデータを受け取り挙動を変えるクラスとなる
#include "main.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "enemy/enemy_spawner.h"
#include "collider/sphere.h"
#include "player.h"

void FieldEnemy::Init(EnemyBase* base, Transform trans)
{
	// 基礎データをセット
	SetEnemyBase(base);

	// 初期座標で設定する
	SetTransform(trans);

	// 初期化処理
	// コリジョンを有効化する
	Transform transform;
	transform.SetPosition(GetPosition());
	transform.SetScale(COLLIDER_SCALE);

	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform);

	// UV反映(該当enemyのuv位置だけ設定してもらう必要あり)
	// でも敵によって異なるテクスチャ使用する可能性あるし今のdrawみたいな感じの処理にしたほうがいいかな
	// ならuvtexかどうかフラグを作ってそれを用いて分岐かな
	SetCanChangeVertex(); // 多分これ呼ばないとダメ
	ChangeTexUV(base->GetTextureSplitCount().x, base->GetTextureSplitCount().y, base->GetUVPos().x, base->GetUVPos().y);

	//Object3D::Init();
	// テクスチャは敵データから描画時に取得するのでいらない
}

void FieldEnemy::Uninit()
{
	// Spawner側に消えたことを伝える
	Manager::GetCurrentScene()->GetSystemObject<EnemySpawner>()->EnemyKilled();

	// 終了処理
	Object3D::Uninit();
}

void FieldEnemy::Update()
{
	// 生存時間をインクリメント
	m_LiveTime++;

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
			SetDestroy(true);
		}
		// 0以下ならそれ以降の処理は実施したくないのでここでreturn
		return;
	}
	// x,zが-30>=か30<=なら削除
	if (GetPosition().x <= -DELETE_POS.x || GetPosition().x >= DELETE_POS.x ||
		GetPosition().z <= -DELETE_POS.y || GetPosition().z >= DELETE_POS.y)
	{
		SetDestroy(true);
		return;
	}


	// 更新処理
	Object3D::Update();

	// 前fの座標を保存
	UpdatePreviousPosition();
	
	// 各敵のnodeを実行。
	m_EnemyBase->ExecuteAttack(this);
	m_EnemyBase->ExecuteMove(this);

	// コライダ更新
	GetCollider()->Update(GetPosition());

	// 当たってるコライダがあるかチェック
	std::vector<Player*> p_hit = GetCollider()->GetHitObjectsByType<Player>();
	if (!p_hit.empty())
	{
		m_IsHit = true;
	}

	std::vector<FieldEnemy*> e_hit = GetCollider()->GetHitObjectsByType<FieldEnemy>();
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
	//SetDefaultVertexBufferBillboardOnDraw();
	SetVertexBufferOnDraw(); //->inputlayoutやshaderに関してはこのままで良くてここだけ変えないといけない

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