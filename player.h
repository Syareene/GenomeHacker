#pragma once

#include "object/3d_object.h"
#include "enemy/node/base.h"

class Player : public Object3D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	std::list<std::unique_ptr<NodeBase>>& GetAllNodes()
	{
		return m_HavingNodes;
	}
	NodeBase* GetNodeByIndex(const int index)
	{
		if (index < 0 || index >= static_cast<int>(m_HavingNodes.size()))
		{
			return nullptr;
		}
		auto it = m_HavingNodes.begin();
		std::advance(it, index);
		return it->get();
	}
private:
	class ModelRenderer* m_ModelRenderer = nullptr;
	std::list<std::unique_ptr<NodeBase>> m_HavingNodes;
};