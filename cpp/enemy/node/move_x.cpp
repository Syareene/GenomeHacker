#include "main.h"
#include "enemy/node/move_x.h"
#include "enemy/field_enemy.h"


void MoveX::Init(Transform trans)
{
	SetTransform(trans);
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
	m_MoveVal = -0.02f; // 移動量


	// 初期値セット
	m_DescFontData.fontSize = 60;
	m_DescFontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	m_DescFontData.Color = D2D1::ColorF(D2D1::ColorF::Red);
	m_DescFontData.font = DirectWriteCustomFont::GetFontName(0);
	m_DescFontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	m_DescFontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	m_DescFontData.outlineWidth = 6.0f;
}

void MoveX::Uninit()
{

}

void MoveX::Update()
{

}

void MoveX::Draw()
{

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