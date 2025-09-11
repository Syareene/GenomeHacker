#pragma once

#include "lib/vector2.h"

// もしかしたら後々rawinputに統合するかも
class Mouse
{
public:
	// 状態更新系はmain側で行う
	void SetRightButtonDown(bool down);
	void SetRightButtonUp(bool up);
	void SetLeftButtonDown(bool down);
	void SetLeftButtonUp(bool up);
	void SetPosition(Vector2 position);
	void SetWheelDiff(int diff);
	// update2種はmanager側から呼ぶ
	static void Update();
	static void UpdateFinal();
	// 状態取得系は参照したいクラスが呼ぶ
	static inline bool IsRightButtonDown() { return m_RightButtonDown; }
	static inline bool IsRightButtonTrigger() { return m_RightButtonTrigger; }
	static inline bool IsLeftButtonDown() { return m_LeftButtonDown; }
	static inline bool IsLeftButtonTrigger() { return m_LeftButtonTrigger; }
	static inline Vector2 GetPosition() { return m_Position; }
	static inline Vector2 GetDiffPosition() { return m_Position - m_OldPosition; }
	static void SetScreenMousePosition(Vector2 pos);
	static bool IsMouseInsideArea(Vector2 startPos, Vector2 endPos);
	static inline int GetWheelDiff() { return m_WheelDiff; }
private:
	static bool m_RightButtonDown; // 右クリックが押されているかどうか
	static bool m_RightButtonUp; // 右クリックが離されたかどうか
	static bool m_RightButtonDownOld; // 1f前の右クリック状態
	static bool m_RightButtonTrigger; // 右クリックが押されたかどうか(押され始めだけ)
	static bool m_LeftButtonDown; // 左クリックが押されているかどうか
	static bool m_LeftButtonUp; // 左クリックが離されたかどうか
	static bool m_LeftButtonDownOld; // 1f前の左クリック状態
	static bool m_LeftButtonTrigger; // 左クリックが押されたかどうか(押され始めだけ)
	static Vector2 m_OldPosition; // 1f前のマウスの位置
	static Vector2 m_Position; // マウスの位置
	static int m_WheelDiff; // ホイールの差分

	// XBUTTON(サイドキー)は一旦後回し
};