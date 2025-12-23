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

	if (m_TitleCam)
	{
		// 中身実装
		m_TitleCam->Move(1.0f);
	}


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

// 視錐台カリング用
bool Camera::CheckView(const Vector3& position, const float radius)
{
	XMMATRIX vp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	XMMATRIX invVp;
	invVp = XMMatrixInverse(nullptr, vp); // 逆行列

	XMFLOAT3 vpos[4];
	vpos[0] = XMFLOAT3(-1.0f, 1.0f, 0.0f); // 左上
	vpos[1] = XMFLOAT3(1.0f, 1.0f, 0.0f);  // 右上
	vpos[2] = XMFLOAT3(-1.0f, -1.0f, 0.0f); // 左下
	vpos[3] = XMFLOAT3(1.0f, -1.0f, 0.0f); // 右下

	XMVECTOR vposv[4];
	XMVECTOR wposv[4];
	XMFLOAT3 wpos[4];
	for(int i = 0; i < 4; i++)
	{
		vposv[i] = XMLoadFloat3(&vpos[i]);
		wposv[i] = XMVector3TransformCoord(vposv[i], invVp);
		XMStoreFloat3(&wpos[i], wposv[i]);
	}

	// カメラからターゲットに向かうベクトル
	Vector3 v;
	v = position - GetPosition();
	
	Vector3 wp[4];
	for(int i = 0; i < 4; i++)
	{
		wp[i] = Vector3(wpos[i].x, wpos[i].y, wpos[i].z);
	}

	// 左面の判定
	{
		Vector3 v1, v2;
		v1 = wp[0] - GetPosition();
		v2 = wp[2] - GetPosition();
		// 外積を使用して法線ベクトルを求める
		Vector3 n;
		n = CrossProduct(v1, v2);
		n.normalize(); // 正規化

		// 内積を使用して判定
		float d;
		d = n * v;
		// ここの0.0fを-サイズにすると球で判定できるので点判定ではなくなる
		if (d < -radius)
		{
			// 当たってない
			return false;
		}
	}

	// 右面の判定
	{
		Vector3 v1, v2;
		v1 = wp[3] - GetPosition();
		v2 = wp[1] - GetPosition();
		// 外積を使用して法線ベクトルを求める
		Vector3 n;
		n = CrossProduct(v1, v2);
		n.normalize();
		// 内積を使用して判定
		float d;
		d = n * v;
		if (d < -radius)
		{
			// 当たってない
			return false;
		}
	}

	// 距離カリング

	float length = (position - GetPosition()).length();
	if (length > 30.0f)
	{
		return false;
	}
	// このあと外で計算して距離に応じてα値いじるとかにすると見た目良くなる

	return true;
}