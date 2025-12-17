#pragma once

#include "enemy/node/base.h"

class MoveZ : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) { m_MoveVal = val; }

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

private:
	float m_MoveVal = 1.0f; // 動く量
	static NodeTextData m_NodeName; // ノード名
	static std::vector<NodeTextData> m_Descriptions; // ノードの説明部分
	static FontData m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)
};