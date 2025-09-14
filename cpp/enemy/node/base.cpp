#include "enemy/node/base.h"

void NodeBase::Init(Transform trans)
{

}

void NodeBase::Uninit()
{

}

void NodeBase::Update()
{

}

void NodeBase::Draw()
{

}

bool NodeBase::NodeEffect(FieldEnemy* enemy_ptr)
{
	return false;
}

const bool NodeBase::CanAttach(NodeBase* upper_node, NodeBase* lower_node) const
{
	bool canTop = false;
	bool canBottom = false;
	bool isCheckUpper = false;
	bool isCheckLower = false;

	// ノードが存在しない場合はくっつけられないので判定しない
	if (upper_node != nullptr)
	{
		isCheckUpper = true;
	}

	if( lower_node != nullptr)
	{
		isCheckLower = true;
	}

	// 自身の上部にくっつけられるか
	if (isCheckUpper)
	{
		for (const auto& t : m_InputTypesTop)
		{
			// upper_nodeの下部にくっつけられる形と一致するか
			for (const auto& u : upper_node->GetInputTypesBottom())
			{
				if (t == u)
				{
					canTop = true;
					break;
				}
			}
		}
	}
	// 自身の下部にくっつけられるか
	if (isCheckLower)
	{
		for( const auto& t : m_InputTypesBottom)
		{
			// lower_nodeの上部にくっつけられる形と一致するか
			for (const auto& l : lower_node->GetInputTypesTop())
			{
				if (t == l)
				{
					canBottom = true;
					break;
				}
			}
		}
	}

	// 条件に基づいてreturn
	
	// かたっぽだけチェックする場合
	// 下
	if (!isCheckUpper && isCheckLower)
	{
		return canBottom;
	}
	// 上
	else if (isCheckUpper && !isCheckLower)
	{
		return canTop;
	}
	// 両方
	else if (isCheckUpper && isCheckLower)
	{
		return (canTop && canBottom);
	}

	return false;
}