#include "main.h"
#include "score.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "lib/mouse.h"


void Score::Init(Transform trans)
{
	SetTransform(trans);

	// 頂点データ
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, GetVertexBufferPointer());

	// テクスチャ読み込み
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\number.png"));

	// 座標設定
	SetPosition(Vector3(50.0f, 50.0f, 0.0f)); // 画面右上に配置
	SetScale(Vector3(75.0f, 75.0f, 1.0f)); // サイズを調整
}

void Score::Uninit()
{
	// 開放
	TextureManager::UnloadTexture(GetTextureID());
	UninitDrawMember();
}

void Score::Update()
{
	// 一度仮置きなのでこちら側で判定を取る
	// 指定したエリア内にマウスがいるかどうか(表示している爆弾のスプライトがこのサイズなので仮で此方側で判定を合わせる)
	//bool is_inside = Mouse::IsMouseInsideArea(Vector2(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 300.0f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// エリア内にいるかつクリックしたなら
	//if(is_inside && Mouse::IsLeftButtonTrigger())
	//{
		// スコアを加算
	//	m_Score += 100;
	//}

	SetPosition(Vector3(GetPosition().x, GetPosition().y + (Mouse::GetWheelDiff() * -0.3f), GetPosition().z));
}

void Score::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	SetProjectionMatrixOnDraw();

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 徐々に表示するのでそのスコアを計算用として保存
	int SingleScore = m_Score;
	// スコアがShowableDigits以上の場合は、表示可能な桁数に制限
	if(SingleScore >= static_cast<int>(pow(10, ShowableDigits) + 1))
	{
		SingleScore = static_cast<int>(pow(10, ShowableDigits) - 1); // 最大値を制限
	}

	// 頂点データ書き換え
	for (int i = 0; i < ShowableDigits; i++)
	{
		int show_num = SingleScore % 10; // 現在の桁の数字を取得
		SingleScore /= 10; // 次の桁へ進む
		// 頂点書き換え始め
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float texture_width = 1.0f / 5.0f; // テクスチャの横幅を5分割
		float texture_height = 1.0f / 5.0f; // テクスチャの縦幅を5分割
		float offset_x = (show_num % 5) * texture_width; // フレームに応じたXオフセット
		float offset_y = (show_num / 5) * texture_height; // フレームに応じたYオフセット

		vertex[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = XMFLOAT2(offset_x, offset_y);

		vertex[1].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = XMFLOAT2(offset_x + texture_width, offset_y);

		vertex[2].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = XMFLOAT2(offset_x, offset_y + texture_height);

		vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(offset_x + texture_width, offset_y + texture_height);
		// 頂点書き換え終了
		Renderer::GetDeviceContext()->Unmap(GetVertexBuffer(), 0);

		// 頂点バッファ設定
		SetVertexBufferOnDraw();
		// ビューマトリックス設定
		SetViewMatrixOnDraw();
		// 移動、回転マトリックス設定
		XMMATRIX trans, world, rot, scale;
		// 桁に応じてXを調整(-1はiが0~4分布なので減算させる)
		trans = XMMatrixTranslation(GetPosition().x + 50.0f * (ShowableDigits - i - 1), GetPosition().y, GetPosition().z);
		rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
		scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(world);
		// マテリアル設定
		SetMaterialOnDraw();

		// 描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}