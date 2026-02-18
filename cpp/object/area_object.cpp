#include "main.h"
#include "lib/renderer.h"
#include "object/area_object.h"
#include "manager/texture_manager.h"
#include "manager/shader_manager.h"
#include "collider/sphere.h"
#include "player.h"
#include "enemy/field_enemy.h"
#include "enemy/base_data/enemy_base.h"
#include "enemy/explosion.h"


void AreaObject::SetPipelineState()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::InstancingAlphaVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::InstancingAlphaVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::InstancingPixelShader, NULL, 0);
}

void AreaObject::UpdateGPUData(InstanceBufferData& data)
{
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	rot = XMMatrixRotationRollPitchYaw(GetRadian().x, GetRadian().y, GetRadian().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 結果をdataに格納
	XMStoreFloat4x4(&data.WorldMatrix, XMMatrixTranspose(world));
	// 色設定
	float alpha = std::min(1.0f, m_Duration / 30.0f); // 持続時間が30フレーム未満の間は徐々に透明から不透明へ変化
	data.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, alpha); // 徐々に見えるように
	// uv設定->ここ元頂点データちゃんと見てくれるから元々の頂点データのTexCoordがちゃんとuvテクスチャ用の座標になってればおけ
	data.UVOffset = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AreaObject::Init(Transform trans)
{
	SetTransform(trans);
	SetScale(GetScale() * 2.0f); // デフォルトの大きさを2倍にする

	// 当たり判定をセット
	Transform transform;
	trans = GetTransform();
	//trans.SetScale();
	Sphere* collider = SetCollider<Sphere>();
	collider->Init(transform);

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, 0.0f, 0.5f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, 0.0f, 0.5f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, 0.0f, -0.5f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.0f, -0.5f);
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
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\area.png"));

	AddTag("in_game");
}

void AreaObject::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void AreaObject::Update()
{
	// 登場したては透明な時間作ってその後適応みたいな感じにしたいな


	// カウントインクリメント
	m_Duration++;

	// コライダの場所更新
	GetCollider()->Update(GetPosition());



	// もし持続時間が最大値を超えたら削除
	if (m_Duration >= m_MaxDuration)
	{
		SetDestroy(true);
	}

	// これより下の判定はm_Durationが30以上だけ
	if(m_Duration < 30)
	{
		return; // 持続時間が30未満なら当たり判定を行わない
	}


	// プレイヤーとの衝突判定
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
		enemy->DecreaseHP(m_AreaDamage);
		break; // 一つの敵に当たったらループを抜ける
	}
}
// gpuのやつとか作成する必要あり

void AreaObject::Draw()
{
	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();

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
}