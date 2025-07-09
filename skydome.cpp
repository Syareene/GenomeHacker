#include "main.h"
#include "renderer.h"
#include "skydome.h"
#include "modelRenderer.h"
#include "input.h"
#include "object/camera.h"
#include "manager.h"

void SkyDome::Init()
{
	// モデル読み込み
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\sky.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void SkyDome::Uninit()
{
	delete m_ModelRenderer;
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SkyDome::Update()
{
	// vector3が間違ってる可能性とかもある?

	Camera* camera = Manager::GetCurrentScene()->GetGameObject<Camera>();
	
	SetPosition(camera->GetPosition()); // カメラの位置にドームを合わせる

	// ここonとM_PIでバグるね->いらない?
	//camera->SetTarget(GetPosition() + Vector3(0.0f, 1.5f, 0.0f));
}

void SkyDome::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	XMMATRIX trans, world, rot, scale;
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	//rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y + XM_PI, GetRotation().z);
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}