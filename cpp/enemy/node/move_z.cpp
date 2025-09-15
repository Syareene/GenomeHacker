#include "enemy/node/move_z.h"
#include "enemy/field_enemy.h"
#include "lib/vector3.h"

void MoveZ::Init(Transform trans)
{
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCD(0);
	m_MoveVal = 0.02f; // 移動量
}

void MoveZ::Uninit()
{

}

void MoveZ::Update()
{

}

void MoveZ::Draw()
{

}

bool MoveZ::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	// 定期的に動かす
	enemy_ptr->AddPosition(Vector3(0.0f, 0.0f, m_MoveVal)); // z方向に動かす

	return true;
}