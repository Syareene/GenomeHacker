#include "collider/sphere.h"
#include "main.h"
#include "renderer.h"
#include "shader_manager.h"

ID3D11Buffer* Sphere::m_VertexBuffer = nullptr;

void Sphere::Init()
{
	if(m_VertexBuffer)
	{
		// 既に生成されている
		return;
	}
	// 頂点バッファ生成
	// 頂点は12頂点で円を描く
	std::vector<Vector3> circleVertex;
	MakeCircleVertex(m_CircleVertexCount, circleVertex);
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vector3) * static_cast<UINT>(m_CircleVertexCount + 1); // (+1は円閉じるための描画用)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = circleVertex.data();
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

// これ開放するタイミング考えないとエラー出るわよ
void Sphere::Uninit()
{
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = nullptr;
	}
}

void Sphere::Update()
{
	// 特に何もしない
}


void Sphere::DrawCollider()
{
	// デバッグ時のみコライダを描画
#ifdef _DEBUG
	// 生成した頂点を使用し描画

	// シェーダーは一旦セットしない。後々デバッグ用に色を変えるシェーダー作成する。

	// 入力レイアウト設定(シェーダーのレイアウトなのでこれもシェーダー使用するときに用意)
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::DebugVertexLayout);
	// シェーダー設定->とりあえずunlitだがデバッグ用のを後々用意。
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::DebugVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::DebugPixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetCenter().x, GetCenter().y, GetCenter().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, 1.0f);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// マテリアル設定
	MATERIAL material;
	material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	// specとemissionもあるが一旦放置
	material.TextureEnable = false;
	Renderer::SetMaterial(material);

	// 頂点バッファ設定
	UINT stride = sizeof(Vector3);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// 当たってるならparameterをtrueにする(別に本来はmaterialに対して色変えるでいいんだけどね)
	if (GetIsHit())
	{
		Renderer::SetParameter(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f)); // true
	}
	else
	{
		Renderer::SetParameter(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)); // false
	}

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 描画(+1は円閉じるための描画用)
	Renderer::GetDeviceContext()->Draw(m_CircleVertexCount + 1, 0);

	// 縦には生成できたので回転し横向きにも描画
	
	// 移動、回転マトリックス設定
	trans = XMMatrixTranslation(GetCenter().x, GetCenter().y, GetCenter().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x - 89.5f, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, 1.0f);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// 他は再利用するのでそのまま描画(+1は円閉じるための描画用)
	Renderer::GetDeviceContext()->Draw(m_CircleVertexCount + 1, 0);

	// hitリセット
	SetIsHit(false);
#endif
}

void Sphere::MakeCircleVertex(int vertex_count, std::vector<Vector3>& outVertex)
{
	// Sphereの可視化
	// デバッグ用に球を描画する

	for (int i = 0; i <= vertex_count; i++)
	{
		Vector3 vertex;

		float theta = float(i) / float(vertex_count) * 2.0f * XM_PI;
		float x = 1.0f * cosf(theta);
		float y = 1.0f * sinf(theta);

		outVertex.push_back(Vector3(x, y, 0.0f));
	}

	// 最後に最初の頂点を追加して円を閉じる
	outVertex.push_back(outVertex[0]);
}

bool Sphere::CheckCollision(const Collision& other)
{
	// ここ、IsHit既にtrueならfalseにならないように変えないとダメそう
	bool temp = other.CheckCollisionSphere(*this);
	// 既にどれかのオブジェクトには当たっているが、今回の相手にあたっていない場合は
	// 当たった変数自体はfalseにしてほしくないので変える前に値を返す
	if (GetIsHit() && !temp)
	{
		return temp;
	}

	// 相手の型に応じて処理を分岐
	SetIsHit(temp);
	return temp;
}

bool Sphere::CheckCollisionSphere(const Collision& other) const
{
	// Sphereの中心位置と半径を取得
	Vector3 pos1 = GetCenter();
	Vector3 pos2 = other.GetCenter();
	float radius1 = GetScale().x * 0.5f; // 半径はスケールの一辺の半分と仮定
	float radius2 = other.GetScale().x * 0.5f;
	// 中心間の距離を計算
	Vector3 diff = pos1 - pos2;
	float distanceSquared = diff.lengthSquared();
	// 半径の和を計算
	float radiusSum = radius1 + radius2;

	// 衝突判定
	return distanceSquared <= (radiusSum * radiusSum);
}

bool Sphere::CheckCollisionAABB(const Collision& other) const
{
	// AABBの中心位置と半径を取得

	// こっちはAABB
	Vector3 pos1 = other.GetCenter();
	Vector3 scale1 = other.GetScale();
	Vector3 min1 = pos1 - scale1 * 0.5f;
	Vector3 max1 = pos1 + scale1 * 0.5f;
	Vector3 pos2 = GetCenter();
	float radius2 = GetScale().x * 0.5f; // 半径はスケールの一辺の半分と仮定

	// AABBの中心からSphereの中心までの最近点を計算
	Vector3 closestPoint(
		std::max(min1.x, std::min(pos2.x, max1.x)),
		std::max(min1.y, std::min(pos2.y, max1.y)),
		std::max(min1.z, std::min(pos2.z, max1.z))
	);
	// 最近点とSphereの中心との距離を計算
	Vector3 diff = closestPoint - pos2;
	float distanceSquared = diff.lengthSquared();
	// 衝突判定
	return distanceSquared <= (radius2 * radius2);
}

bool Sphere::CheckCollisionOBB(const Collision& other) const
{
	return false; // 未実装
}