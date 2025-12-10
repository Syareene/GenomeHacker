#pragma once

#include "object/ui/font.h"
#include "lib/write_font.h"

class NodeFontManager
{
	// staticクラス

public:
	enum class TextType
	{
		Description = 0,
		RareDescription,
		ElemLength // ここ要素数として使いたいので最後に
	};

	static void Init();
	static void Draw(std::string text, const Vector2& pos, TextType type);

private:
	// フォント描画するときはFontDataをポインタに取りつつ->でアクセスし、DrawString関数はstringとposあればいいから構造見直し
	static std::vector<FontData> m_FontDataMap;
};