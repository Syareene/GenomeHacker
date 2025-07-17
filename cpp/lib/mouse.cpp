#include "lib/mouse.h"
#include "main.h"

bool Mouse::m_RightButtonDown = false; // 右クリックが押されているかどうか
bool Mouse::m_RightButtonUp = false; // 右クリックが離されたかどうか
bool Mouse::m_RightButtonDownOld = false; // 1f前の右クリック状態
bool Mouse::m_RightButtonTrigger = false; // 右クリックが押されたかどうか(押され始めだけ)
bool Mouse::m_LeftButtonDown = false; // 左クリックが押されているかどうか
bool Mouse::m_LeftButtonUp = false; // 左クリックが離されたかどうか
bool Mouse::m_LeftButtonDownOld = false; // 1f前の左クリック状態
bool Mouse::m_LeftButtonTrigger = false; // 左クリックが押されたかどうか(押され始めだけ)
Vector2 Mouse::m_OldPosition = Vector2(0.0f, 0.0f); // 1f前のマウスの位置
Vector2 Mouse::m_Position = Vector2(0.0f, 0.0f); // マウスの位置


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

// マウスの位置を設定(内部的な値であって実マウスの座標を変えれるわけではない)
void Mouse::SetPosition(Vector2 position)
{
	m_OldPosition = m_Position; // 1f前の位置を保存
	m_Position = position;
}

// 実際のマウスの座標を設定する関数
void Mouse::SetScreenMousePosition(Vector2 pos)
{
	// マウスの位置を画面座標に設定

	// 現在の画面サイズを取得
	Vector2 actual_screen_size = Vector2(static_cast<float>(GetRect().right - GetRect().left), static_cast<float>(GetRect().bottom - GetRect().top));
	// 想定しているウィンドウサイズとの比率を計算
	Vector2 wnd_scale = Vector2(actual_screen_size.x / SCREEN_WIDTH, actual_screen_size.y / SCREEN_HEIGHT);
	// その比率を使ってposを補正する

	// ウィンドウの左上位置を取得
	Vector2 window_start_pos = Vector2(static_cast<float>(GetRect().left), static_cast<float>(GetRect().top));
	// 補正後の位置を計算しマウス座標を設定
	SetCursorPos(static_cast<int>((pos.x * wnd_scale.x) + window_start_pos.x), static_cast<int>((pos.y * wnd_scale.y) + window_start_pos.y));
}

void Mouse::Update()
{
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

bool Mouse::IsMouseInsideArea(Vector2 startPos, Vector2 endPos)
{
	// 現在の画面サイズを取得
	Vector2 actual_screen_size = Vector2(static_cast<float>(GetRect().right - GetRect().left), static_cast<float>(GetRect().bottom - GetRect().top));
	// 想定しているウィンドウサイズとの比率を計算
	Vector2 wnd_scale = Vector2(actual_screen_size.x / SCREEN_WIDTH, actual_screen_size.y / SCREEN_HEIGHT);
	// その比率を使ってposを補正する

	// マウスの位置が指定された範囲内にあるかどうかをチェック
	return (m_Position.x >= startPos.x * wnd_scale.x && m_Position.x <= endPos.x * wnd_scale.x &&
			m_Position.y >= startPos.y * wnd_scale.y && m_Position.y <= endPos.y * wnd_scale.y);
}