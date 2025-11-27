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
}

void AddScore::Uninit()
{

}

void AddScore::Update()
{

}

void AddScore::Draw()
{

}

bool AddScore::NodeEffect(FieldEnemy* enemy_ptr)
{
	// スコア加算
	Manager::GetCurrentScene()->GetGameObject<Score>()->AddScore(static_cast<int>(m_AddScore));

	return true;
}