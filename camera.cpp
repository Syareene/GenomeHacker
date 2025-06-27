#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "object/camera.h"
#include "player.h"
#include "input.h"

void Camera::Init()
{
	SetPosition(Vector3(0.0f, 1.0f, -5.0f));
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	if (Input::GetKeyPress(VK_LEFT))
	{
		SetRotation(GetRotation() + Vector3(0.0f, -0.05f, 0.0f));
	}

	if (Input::GetKeyPress(VK_RIGHT))
	{
		SetRotation(GetRotation() + Vector3(0.0f, 0.05f, 0.0f));
	}

	/*
	if (Input::GetKeyPress(VK_UP))
	{
		SetPosition(GetPosition() + Vector3(0.0f, 0.0f, 0.1f));
	}

	if (Input::GetKeyPress(VK_DOWN))
	{
		SetPosition(GetPosition() + Vector3(0.0f, 0.0f, -0.1f));
	}
	*/

	Player* player = Manager::GetCurrentScene()->GetGameObject<Player>();

	m_Target = player->GetPosition() + Vector3(0.0f, 1.5f, 0.0f);
	SetPosition(m_Target + Vector3(-sinf(GetRotation().y), 0.5f, -cosf(GetRotation().y)) * 5.0f);
	
	// 極座標変換で上向きも対応
	//float r = sqrtf(GetRotation().x + GetRotation().y + GetRotation().z);
	//float theta = acosf(GetRotation)

	//SetPosition(m_Target + Vector3(sqrtf(GetRotation().x + GetRotation().y + GetRotation().z)) 

	// 上下も考慮
	//SetPosition(GetPosition() + Vector3(0.0f, sinf(GetRotation().x), 0.0f) * 5.0f);
}

void Camera::Draw()
{
	// プロジェクションマトリックス
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	Renderer::SetProjectionMatrix(m_ProjectionMatrix);
	// ビューマトリックス
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	m_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&GetPosition()), XMLoadFloat3((XMFLOAT3*)&m_Target), XMLoadFloat3(&up));
	Renderer::SetViewMatrix(m_ViewMatrix);
}