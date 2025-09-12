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

bool NodeBase::NodeEffect()
{
	return false;
}

// これinsertする場合はどうしようね、上下判定しないといけないから
const bool NodeBase::CanAttach(InputType& type) const
{
	for (const auto& t : m_InputTypesBottom)
	{
		if (t == type)
		{
			return true;
		}
	}
	return false;
}