#include "main.h"
#include "scene/manager.h"
#include "lib/renderer.h"
#include "object/camera.h"
#include "player.h"
#include "lib/input.h"

void Camera::Init(Transform trans)
{
	SetTransform(trans);
	// 初期位置は少し後ろに
	SetPosition(Vector3(0.0f, 1.0f, -5.0f));
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	if (Input::GetKeyPress(VK_LEFT))
	{
		SetRotation(GetRotation() + Vector3(0.0f, -2.0f, 0.0f));
	}

	if (Input::GetKeyPress(VK_RIGHT))
	{
		SetRotation(GetRotation() + Vector3(0.0f, 2.0f, 0.0f));
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
	if (player == nullptr)
	{
		return;
	}

	m_Target = player->GetPosition() + Vector3(0.0f, 1.5f, 0.0f);
	// カメラシェイク(ここ汎用的じゃないから後々書き直そうかな)
	m_Target += m_ShakeVector * sinf(m_ShakeTime);
	m_ShakeTime += XM_2PI / 3.0f; // 揺らす速さ
	m_ShakeVector *= 0.9f;

	SetPosition(m_Target + Vector3(-sinf(GetRadian().y), 0.5f, -cosf(GetRadian().y)) * 5.0f);
	
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

void Camera::Shake(const Vector3& power)
{
	m_ShakeVector = power;
	m_ShakeTime = 0.0f;
}
