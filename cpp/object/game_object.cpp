#include "object/gameObject.h"
#include "manager.h"
#include "object/camera.h"
#include "default_vertex.h"


void GameObject::UninitDrawMember()
{
	if (m_VertexBuffer) m_VertexBuffer->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();
	if (m_VertexLayout) m_VertexLayout->Release();
	m_VertexBuffer = nullptr;
	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_VertexLayout = nullptr;
}

void GameObject::SetDefaultVertex()
{
	m_VertexBuffer = DefaultVertex::GetBuffer();
}

void GameObject::SetCanChangeVertex()
{
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void GameObject::SetVertexBufferOnDraw() const
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
}

void GameObject::SetProjectionMatrixOnDraw()
{
	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	Renderer::SetProjectionMatrix(projection);
}



void GameObject::SetWorldMatrixOnDraw()
{
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);
}

void GameObject::SetWorldMatrixOnDrawBillboard()
{
	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();
	//XMMATRIX projection, view;

	// ビューの逆行列(ビルボード描画のため)
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, camera->GetViewMatrix());
	invView.r[3].m128_f32[0] = 0.0f; // カメラの位置を無視
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f; // カメラの位置を無視

	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(world);
}

void GameObject::SetViewMatrixOnDraw()
{
	XMMATRIX view;
	view = XMMatrixIdentity();
	Renderer::SetViewMatrix(view);
}

void GameObject::SetMaterialOnDraw(XMFLOAT4 diff, XMFLOAT4 amb, bool texEnable)
{
	MATERIAL material;
	material.Diffuse = diff;
	material.Ambient = amb;
	// specとemissionもあるが一旦放置
	material.TextureEnable = texEnable;
	Renderer::SetMaterial(material);
}

Vector3 GameObject::GetRight() const
{
	// 回転行列から右方向のベクトルを取得
	XMMATRIX matrix;
	matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	// XMFLOAT3に変換してからVector3に変換
	Vector3 right;
	XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);
	return right;
}
Vector3 GameObject::GetUp() const
{
	// 回転行列から上方向のベクトルを取得
	XMMATRIX matrix;
	matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	// XMFLOAT3に変換してからVector3に変換
	Vector3 up;
	XMStoreFloat3((XMFLOAT3*)&up, matrix.r[1]);
	return up;
}

Vector3 GameObject::GetForward() const
{
	// 回転行列から前方向のベクトルを取得
	XMMATRIX matrix;
	matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	// XMFLOAT3に変換してからVector3に変換
	Vector3 forward;
	XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);
	return forward;
}

float GameObject::GetDistance(Vector3 Position)
{
	return (Position - m_Position).length();
}