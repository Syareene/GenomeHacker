#pragma once

#include "object/ui/ui.h"
#include "lib/write_font.h"
#include <memory>

// ui用フォントなため2d継承で実装。
class Font : public UI
{
public:
	Font() = default; // デフォルトコンストラクタ
	virtual ~Font() 
	{

	}
	Font(Font&& Other) noexcept
		: UI(std::move(Other))
		, m_WidthHeight(std::exchange(Other.m_WidthHeight, Vector2(0.0f, 0.0f)))
		, m_FontData(std::move(Other.m_FontData))
		, m_DisplayText(std::move(Other.m_DisplayText))
		, m_IsShadow(std::exchange(Other.m_IsShadow, false))
		, m_IsOutline(std::exchange(Other.m_IsOutline, false))
	{
	}
	Font& operator=(Font&& Other) noexcept
	{
		if (this != &Other)
		{
			UI::operator=(std::move(Other));
			m_WidthHeight = Other.m_WidthHeight;
			m_FontData = std::move(Other.m_FontData);
			m_DisplayText = std::move(Other.m_DisplayText);
			m_IsShadow = Other.m_IsShadow;
			m_IsOutline = Other.m_IsOutline;
		}
		return *this;
	}
	static constexpr bool ENABLE_INSTANCING = false; // インスタンスレンダリング無効
	void Register(const Vector2& pos, const FontData& font_data, std::string text);
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetDisplayText(const std::string& text);
	void GetDisplayText(std::string& outText) const;
	Vector2 GetWidthHeight() const { return m_WidthHeight; }
private:
	Vector2 m_WidthHeight = Vector2(0.0f, 0.0f);
	FontData m_FontData;
	std::string m_DisplayText = ""; // 直接中身いじらず関数使っていじってね(自クラス内でも)
	bool m_IsShadow = false;
	bool m_IsOutline = false;
};