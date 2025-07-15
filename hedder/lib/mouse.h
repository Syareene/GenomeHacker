#pragma once

#include "vector3.h"

// もしかしたら後々rawinputに統合するかも
class Mouse
{
public:
	// 状態更新系はmain側で行う
	void SetRightButtonDown(bool down);
	void SetRightButtonUp(bool up);
	void SetLeftButtonDown(bool down);
	void SetLeftButtonUp(bool up);
	void SetPosition(Vector3 position);
	// update2種はmanager側から呼ぶ
	static void Update();
	static void UpdateFinal();
	// 状態取得系は参照したいクラスが呼ぶ
	static bool IsRightButtonDown() { return m_RightButtonDown; }
	static bool IsRightButtonTrigger() { return m_RightButtonTrigger; }
	static bool IsLeftButtonDown() { return m_LeftButtonDown; }
	static bool IsLeftButtonTrigger() { return m_LeftButtonTrigger; }
	static Vector3 GetPosition() { return m_Position; }
private:
	static bool m_RightButtonDown; // 右クリックが押されているかどうか
	static bool m_RightButtonUp; // 右クリックが離されたかどうか
	static bool m_RightButtonDownOld; // 1f前の右クリック状態
	static bool m_RightButtonTrigger; // 右クリックが押されたかどうか(押され始めだけ)
	static bool m_LeftButtonDown; // 左クリックが押されているかどうか
	static bool m_LeftButtonUp; // 左クリックが離されたかどうか
	static bool m_LeftButtonDownOld; // 1f前の左クリック状態
	static bool m_LeftButtonTrigger; // 左クリックが押されたかどうか(押され始めだけ)
	static Vector3 m_Position; // マウスの位置

	// XBUTTON(サイドキー)は一旦後回し
};