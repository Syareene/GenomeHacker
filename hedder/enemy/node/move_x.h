#pragma once

#include "enemy/node/base.h"

// 多分ui周りのクラス継承?わからんけど
class MoveX : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) { m_MoveVal = val; }
	inline const std::vector<NodeDescription*> GetDescriptions() const override
	{
		std::vector<NodeDescription*> descs;
		for(auto& desc_ptr : m_Description)
		{
			descs.push_back(desc_ptr.get());
		}
		return descs;
	}

	inline const NodeDescription* GetDescription(const int index) const override
	{
		return m_Description[index].get();
	}

	inline void AddDescription(const NodeDescription& desc) override
	{
		// desc使うようにする
		std::unique_ptr<NodeDescription> desc_ptr = std::make_unique<NodeDescription>();
		m_Description.push_back(std::move(desc_ptr));
	}

	inline void SetDescriptionFontData(const FontData& fontData) override
	{
		m_DescFontData = fontData;
	}

	inline FontData& GetDescriptionFontData() override
	{
		return m_DescFontData;
	}

	// dna_editに行った時に表示するフォントオブジェクト郡(Fontの詳細な色とかはm_DescFontDataから引っ張る)
	inline void AddFont(const std::string& text, const Vector2& pos) override
	{
		// 既存のfontdataを利用してfontオブジェクトを生成
		std::unique_ptr<Font> font_ptr = std::make_unique<Font>();
		font_ptr->Init(Transform());
		font_ptr->Register(pos, m_DescFontData, text);
	}
	inline const std::vector<Font*> GetFonts() override
	{
		std::vector<Font*> fonts;
		for (auto& font_ptr : m_DescriptionFonts)
		{
			fonts.push_back(font_ptr.get());
		}
		return fonts;
	}

private:
	float m_MoveVal = 1.0f; // 動く量
	static std::vector<std::unique_ptr<NodeDescription>> m_Description; // ノードの説明部分
	static std::vector<std::unique_ptr<Font>> m_DescriptionFonts; // dna_editに行った時に表示するフォントオブジェクト郡
	static FontData m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)
};