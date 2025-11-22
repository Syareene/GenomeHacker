#pragma once

#include "ui.h"

#include "object/ui/font.h"

class Button : public UI
{
private:
	std::function<void()> m_TargetFunc;
	int m_FrameTexID = -1; // フレームテクスチャID
	std::unique_ptr<Font> m_Text = nullptr;
public:
	virtual ~Button() {}
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const std::wstring& filePath, const std::wstring& frameTexPath = L"");
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const int& texID, const std::wstring frameTexPath = L"");
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const FontData& fontData, const std::string& text, const std::wstring& filePath = L"", const std::wstring& frameTexPath = L"");
	void Init(Transform trans = Transform()) override; // ボタン登録処理
	// ボタンが押されたときに実行したいオブジェクト(コールバック登録)
	template<typename... Args>
	inline void SetOnClick(const std::function<void()>& func)
	{
		m_TargetFunc = func; // 関数を設定
	}
	
	inline void OnClick()
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