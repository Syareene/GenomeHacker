#include "main.h"
#include "enemy/node/number.h"


void NumberNode::Init(Transform trans)
{
	SetTransform(trans);
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

void NumberNode::Uninit()
{

}

void NumberNode::Update()
{

}

void NumberNode::Draw()
{

}

bool NumberNode::NodeEffect(FieldEnemy* enemy_ptr)
{
	// moveノードなのでcdはチェックせず常に動かす
	// 通常のノードならcdチェックし動かす形に
	// んで実行したらtrueを返す

	// 定期的に動かす
	//enemy_ptr->AddPosition(Vector3(0.0f, 0.0f, m_MoveVal)); // z方向に動かす

	return true;
}