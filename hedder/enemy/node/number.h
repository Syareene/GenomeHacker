#pragma once

//#include <list>
#include "enemy/node/base.h"
#include "enemy/field_enemy.h"

// 多分ui周りのクラス継承?わからんけど
class NumberNode : public NodeBase
{
public:
	// これ考えたけどsprite作るのがめんどくさいから形みたいな感じじゃなくて内部処理でくっつくくっつかないとかになるのかな？〇〇の場所で使えるノードみたいな感じで

	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override; // ->基本nodeeffectで良さそうではあるが、、
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override;

	//inline const std::vector<NodeDescription*> GetDescriptions() const override
	//{
	//	std::vector<NodeDescription*> descs;
	//	for (auto& desc_ptr : m_Description)
	//	{
	//		descs.push_back(desc_ptr.get());
	//	}
	//	return descs;
	//}

	//inline const NodeDescription* GetDescription(const int index) const override
	//{
	//	return m_Description[index].get();
	//}

	//inline void AddDescription(const NodeDescription& desc) override
	//{
	//	// desc使うようにする
	//	std::unique_ptr<NodeDescription> desc_ptr = std::make_unique<NodeDescription>();
	//	m_Description.push_back(std::move(desc_ptr));
	//}

	inline void SetDescriptionFontData(const FontData& fontData) override
	{
		m_DescFontData = fontData;
	}

	inline FontData& GetDescriptionFontData() override
	{
		return m_DescFontData;
	}

	void UpdateDescriptionData() override;

private:
	static NodeTextData m_NodeName; // ノード名
	static std::vector<NodeTextData> m_Descriptions; // ノードの説明部分
	static FontData m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)
};