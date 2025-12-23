#pragma once

#include "enemy/node/base.h"

class AddScore: public NodeBase
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override; // 描画時はサイズのプロパティ見てテクスチャとサイズを決める
	bool NodeEffect(FieldEnemy* enemy_ptr) override; // cd管理して終わったならtrueを返す
	void SetAddScore(const float score) 
	{ 
		m_AddScore = score;
		// データを更新したため説明文も更新
		UpdateDescriptionData();
	}

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
	float m_AddScore = 1.0f; // スコア加算量
	static NodeTextData m_NodeName; // ノード名
	static std::vector<NodeTextData> m_Descriptions; // ノードの説明部分
	static FontData m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)
};