#include "enemy/node/add_score.h"
#include "scene/manager.h"
#include "score.h"

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