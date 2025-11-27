#include "main.h"
#include "enemy/node/add_score.h"
#include "scene/manager.h"
#include "score.h"
#include "enemy/node/base.h"

std::vector<std::unique_ptr<NodeBase::NodeDescription>> AddScore::m_Description; // ノードの説明部分
std::vector<std::unique_ptr<Font>> AddScore::m_DescriptionFonts; // dna_editに行った時に表示するフォントオブジェクト郡
FontData AddScore::m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)

void AddScore::Init(Transform trans)
{
	SetTransform(trans);
	AddInputTypeTop(InputType::Death);
	AddInputTypeBottom(InputType::Death);
	SetCDMax(0);
	SetCD(0);
	m_AddScore = 1.0f; // スコア加算量

	// 初期値セット
	m_DescFontData.fontSize = 24;
	m_DescFontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_DescFontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_DescFontData.font = DirectWriteCustomFont::GetFontName(0);
	m_DescFontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_DescFontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.outlineWidth = 2.5f;

	// 生成されていないなら説明文をセット
	if (m_DescriptionFonts.size() == 0)
	{
		// 説明文セット
		m_DescriptionFonts.push_back(std::make_unique<Font>());
		m_DescriptionFonts.back()->Init(Transform());
		m_DescriptionFonts.back()->Register(Vector2(10.0f, 300.0f), m_DescFontData, "Number: このノードを通過するとスコアがnだけ加算されます。");
	}
}

void AddScore::Uninit()
{
	NodeBase::Uninit();
}

void AddScore::Update()
{
	NodeBase::Update();
}

void AddScore::Draw()
{
	NodeBase::Draw();

	// テキストを描画
	for (auto& font_ptr : m_DescriptionFonts)
	{
		font_ptr->Draw();
	}
}

bool AddScore::NodeEffect(FieldEnemy* enemy_ptr)
{
	// スコア加算
	Manager::GetCurrentScene()->GetGameObject<Score>()->AddScore(static_cast<int>(m_AddScore));

	return true;
}