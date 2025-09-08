#pragma once

#include "ui.h"
#include "lib/vector2.h"

class Button : public UI
{
private:
	std::function<void()> m_TargetFunc;
	int m_FrameTexID = -1; // フレームテクスチャID
public:
	virtual ~Button() { OutputDebugStringA("~Button\n"); }
	void Register(const std::function<void()>& func, Vector2 pos, Vector2 scale, Vector2 rot, const std::wstring filePath, const std::wstring frameTexPath = L"");
	void Register(const std::function<void()>& func, Vector2 pos, Vector2 scale, Vector2 rot, int texID, const std::wstring frameTexPath = L"");
	void Init(Transform trans = Transform()) override; // ボタン登録処理
	// ボタンが押されたときに実行したいオブジェクト(コールバック登録)
	template<typename... Args>
	void SetOnClick(const std::function<void()>& func)
	{
		m_TargetFunc = func; // 関数を設定
	}
	
	void OnClick()
	{
		if(m_TargetFunc) // 関数が設定されている場合
		{
			m_TargetFunc(); // 関数を実行
		}
	}
	void Uninit() override; // ボタン終了処理
	void Update() override; // ボタン更新処理
	void Draw() override; // ボタン描画処理
};