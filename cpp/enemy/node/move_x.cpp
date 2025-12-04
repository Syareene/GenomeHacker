#include "main.h"
#include "enemy/node/move_x.h"
#include "enemy/field_enemy.h"

std::vector<std::unique_ptr<NodeBase::NodeDescription>> MoveX::m_Description; // ノードの説明部分
std::vector<std::unique_ptr<Font>> MoveX::m_DescriptionFonts; // dna_editに行った時に表示するフォントオブジェクト郡
FontData MoveX::m_DescFontData; // 説明文用のフォントデータ(クラス内で共通利用したいため)

void MoveX::Init(Transform trans)
{
	Transform defaultTrans = Transform();
	defaultTrans.SetScale(Vector3(500.0f, 100.0f, 0.0f));
	defaultTrans.SetPosition(Vector3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	NodeBase::Init(defaultTrans);
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
	m_MoveVal = -0.02f; // 移動量

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
	if(m_DescriptionFonts.size() == 0)
	{
		// 説明文セット
		m_DescriptionFonts.push_back(std::make_unique<Font>());
		m_DescriptionFonts.back()->Init(Transform());
		m_DescriptionFonts.back()->Register(Vector2(10.0f, 300.0f), m_DescFontData, "MoveX: このノードがある敵は毎フレームn分だけX軸に対し移動します。");
	}
}

void MoveX::Uninit()
{
	NodeBase::Uninit();
}

void MoveX::Update()
{
	NodeBase::Update();
}

void MoveX::Draw()
{
	NodeBase::Draw();

	// テキストを描画
	for (auto& font_ptr : m_DescriptionFonts)
	{
		font_ptr->Draw();
	}
}

bool MoveX::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	// 定期的に動かす
	enemy_ptr->AddPosition(Vector3(m_MoveVal, 0.0f, 0.0f)); // x方向に動かす

	return true;
}