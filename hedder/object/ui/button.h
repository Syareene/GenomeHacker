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
	Button() = default; // デフォルトコンストラクタ
	virtual ~Button() {}

	Button(Button&& Other) noexcept
		: UI(std::move(Other))
		, m_TargetFunc(std::move(Other.m_TargetFunc))
		, m_FrameTexID(Other.m_FrameTexID)
		, m_Text(std::move(Other.m_Text))
	{
		Other.m_FrameTexID = -1;
		// ムーブ後の Other.m_TargetFunc は std::move により空になる
	}
	Button& operator=(Button&&) noexcept = default; // ムーブ代入演算子
	static constexpr bool ENABLE_INSTANCING = false; // インスタンスレンダリング無効(汎用クラスは別テクスチャ読み込むのに対応してないので一旦無効化)
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const std::wstring& filePath, const std::wstring& frameTexPath = L"");
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const int& texID, const std::wstring frameTexPath = L"");
	void Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const FontData& fontData, const std::string& text, const std::wstring& filePath = L"", const std::wstring& frameTexPath = L"");
	virtual void Init(Transform trans = Transform()); // ボタン登録処理
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