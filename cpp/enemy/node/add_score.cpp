#include "main.h"
#include "enemy/node/add_score.h"
#include "scene/manager.h"
#include "score.h"
#include "enemy/node/base.h"

NodeBase::NodeTextData AddScore::m_NodeName; // ノード名
std::vector<NodeBase::NodeTextData> AddScore::m_Descriptions; // ノードの説明部分
FontData AddScore::m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)

void AddScore::Init(Transform trans)
{
	Transform defaultTrans = Transform();
	defaultTrans.SetScale(Vector3(500.0f, 100.0f, 0.0f));
	defaultTrans.SetPosition(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	// 初期値セット
	m_DescFontData.fontSize = 24;
	m_DescFontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_DescFontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_DescFontData.font = DirectWriteCustomFont::GetFontName(0);
	m_DescFontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_DescFontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.outlineWidth = 2.5f;

	// ベースデータセット
	m_NodeName = { "AddScore", Vector2(10.0f, 10.0f) };

	m_Descriptions.push_back({ "このノードを通過するとスコアがnだけ加算されます。", Vector2(10.0f, 300.0f) });

	// 生成されていないなら説明文をセット
	if (GetDescFonts().size() == 0)
	{
		// ノード名セット
		SetNameFont(m_DescFontData, m_NodeName.description);

		// 説明文セット
		for (auto& desc : m_Descriptions)
		{
			AddDescFont(m_DescFontData, desc.description);
		}
	}

	// フォント作られてから基底クラスのinitを呼ぶ(textのポインタを取得したいので)
	NodeBase::Init(defaultTrans);
	AddInputTypeTop(InputType::Death);
	AddInputTypeBottom(InputType::Death);
	SetCDMax(0);
	SetCD(0);
	m_AddScore = 1.0f; // スコア加算量
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
	//for (auto& font_ptr : m_DescriptionFonts)
	//{
	//	font_ptr->Draw();
	//}
}

bool AddScore::NodeEffect(FieldEnemy* enemy_ptr)
{
	// スコア加算
	Manager::GetCurrentScene()->GetGameObject<Score>()->AddScore(static_cast<int>(m_AddScore));

	return true;
}