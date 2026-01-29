#include "main.h"
#include "enemy/node/number.h"


void NumberNode::Init(Transform trans)
{
	AddInputTypeTop(InputType::Move);
	AddInputTypeBottom(InputType::Move);
	SetCDMax(0);
	SetCD(0);
}

bool NumberNode::NodeEffect(FieldEnemy* enemy_ptr)
{

	return true;
}

std::string NumberNode::GenerateDescriptionText()
{
	// 説明文のテンプレートを取得
	std::string format_string = "このノードは数字ノードです。";
	return format_string;
}