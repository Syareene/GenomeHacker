// 基礎データからデータを持ってきてフィールドに描画する
// データを読み取ってから書くのでこのクラスは継承されずデータを受け取り挙動を変えるクラスとなる
#include "main.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "enemy/enemy_spawner.h"
#include "collider/sphere.h"
#include "object/player.h"
#include "object/camera.h"

void FieldEnemy::SetPipelineState()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::InstancingVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::InstancingVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::InstancingPixelShader, NULL, 0);

	// プリミティブトポロジ
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void FieldEnemy::UpdateGPUData(InstanceBufferData& data)
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
	Vector2 uv_pos = CalcTexUVOffset(static_cast<int>(GetEnemyBase()->GetTextureSplitCount().x), static_cast<int>(GetEnemyBase()->GetTextureSplitCount().y), 
																static_cast<int>(GetEnemyBase()->GetUVPos().x), static_cast<int>(GetEnemyBase()->GetUVPos().y));
	// 敵ごとにテクスチャが異なるためシェーダー側でその値をずらしてあげる
	data.UVOffset = XMFLOAT4(uv_pos.x, uv_pos.y, 0.0f, 0.0f);
}

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

	// UV反映
	SetCanChangeVertex(false);
	ChangeTexUV(static_cast<int>(base->GetTextureSplitCount().x), static_cast<int>(base->GetTextureSplitCount().y), 0, 0, false);
	// textureIDをセット
	SetTextureID(base->GetEnemyTextureID());
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
	// 敵データからテクスチャを取得して描画

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
	SetVertexBufferOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする

	ID3D11ShaderResourceView* texture = TextureManager::Get3DTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	// コリジョン描画(デバッグ用)
	/*if(!GetCollider())
	{
		return;
	}
	GetCollider()->DrawCollider();*/
}