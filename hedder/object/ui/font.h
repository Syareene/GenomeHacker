#pragma once

#include "object/ui/ui.h"
#include "lib/write_font.h"
#include <memory>

// これ、3d空間上に出したいとかならちょっと仕組み変えないといけないけど
// とりあえずui用フォントなため2d継承で実装。
class Font : public UI
{
public:
	Font() = default; // デフォルトコンストラクタ
	virtual ~Font() {}
	Font(Font&&) noexcept = default; // ムーブコンストラクタ
	Font& operator=(Font&&) noexcept = default; // ムーブ代入演算子
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