#include "lib/mouse.h"

bool Mouse::m_RightButtonDown = false; // 右クリックが押されているかどうか
bool Mouse::m_RightButtonUp = false; // 右クリックが離されたかどうか
bool Mouse::m_RightButtonDownOld = false; // 1f前の右クリック状態
bool Mouse::m_RightButtonTrigger = false; // 右クリックが押されたかどうか(押され始めだけ)
bool Mouse::m_LeftButtonDown = false; // 左クリックが押されているかどうか
bool Mouse::m_LeftButtonUp = false; // 左クリックが離されたかどうか
bool Mouse::m_LeftButtonDownOld = false; // 1f前の左クリック状態
bool Mouse::m_LeftButtonTrigger = false; // 左クリックが押されたかどうか(押され始めだけ)
Vector3 Mouse::m_Position = Vector3(0.0f, 0.0f, 0.0f); // マウスの位置


void Mouse::SetRightButtonDown(bool down)
{
	m_RightButtonDown = down;
}

void Mouse::SetRightButtonUp(bool up)
{
	m_RightButtonUp = up;
}

void Mouse::SetLeftButtonDown(bool down)
{
	m_LeftButtonDown = down;
}

void Mouse::SetLeftButtonUp(bool up)
{
	m_LeftButtonUp = up;
}

// マウスの位置を設定
void Mouse::SetPosition(Vector3 position)
{
	m_Position = position;
}

void Mouse::Update()
{
	// クリック状態や動かした状態はwndのメッセージから更新済み

	// トリガーの状態更新
	m_RightButtonTrigger = m_RightButtonDown && !m_RightButtonDownOld; // 右クリックが押された瞬間
	m_LeftButtonTrigger = m_LeftButtonDown && !m_LeftButtonDownOld; // 左クリックが押された瞬間

	// 1f前の状態を保存
	m_RightButtonDownOld = m_RightButtonDown;
	m_LeftButtonDownOld = m_LeftButtonDown;
}

void Mouse::UpdateFinal()
{
	// 最後に呼ぶやつ

	// 状態フラグを一律falseへ
	m_LeftButtonDown = false;
	m_LeftButtonUp = false;
	m_RightButtonDown = false;
	m_RightButtonUp = false;
}