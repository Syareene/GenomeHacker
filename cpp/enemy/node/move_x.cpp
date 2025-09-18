#include "enemy/node/move_x.h"
#include "enemy/field_enemy.h"
#include "lib/vector3.h"

void MoveX::Init(Transform trans)
{
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
	m_MoveVal = -0.02f; // 移動量
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