#pragma once

#include "enemy/node/base.h"

class MoveX : public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetMoveVal(const float val) { m_MoveVal = val; }
	// ここ基底クラス周りの諸々がめんどくさい
	//inline const std::vector<NodeTextData> GetDescriptions() const override
	//{
	//	std::vector<NodeTextData> descs;
	//	for(const auto& desc : m_Descriptions)
	//	{
	//		descs.push_back(desc);
	//	}
	//	return descs;
	//}

	//inline const NodeTextData GetDescription(const int index) const override
	//{
	//	return m_Descriptions[index];
	//}

	//inline void AddDescription(const NodeTextData& desc) override
	//{
	//	// desc使うようにする
	//	m_Descriptions.push_back(desc);
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