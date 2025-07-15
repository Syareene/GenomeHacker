#pragma once


class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

	// 改良版を下記に
	// 一旦使わないがraw inputを使うようにすればより使いやすい形で作れそう。
	// ただ条件としてはゲーム内側で動きを管理したい(感度設定を追加したり)や、ゲーム側でカスタムされたポインタとかを使う場合になるかな。
};