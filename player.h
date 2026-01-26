#pragma once

#include "object/3d_object.h"
#include "enemy/node/base.h"
#include "enemy/node/visual_base.h"

class Player : public Object3D
{
public:
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;

	inline void SetDnaScreenId(const unsigned int id){m_DnaScreenId = id;}

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
		return (*it).get();
	}
	std::list<std::unique_ptr<VisualBase>>& GetAllVisualNodes()
	{
		return m_NodeVisuals;
	}
	VisualBase* GetVisualNodeByIndex(const int index)
	{
		if (index < 0 || index >= static_cast<int>(m_NodeVisuals.size()))
		{
			return nullptr;
		}
		auto it = m_NodeVisuals.begin();
		std::advance(it, index);
		return (*it).get();
	}
	// この関数引数&なのと、init入れるかどうか
	void AddVisualNode(const VisualBase& visual)
	{
		m_NodeVisuals.emplace_back(std::make_unique<VisualBase>());
		m_NodeVisuals.back()->Init(m_DnaScreenId, static_cast<int>(m_NodeVisuals.size() - 1), visual.GetBaseNodePtr());
	}
private:
	class ModelRenderer* m_ModelRenderer = nullptr;
	std::list<std::unique_ptr<NodeBase>>m_HavingNodes;
	std::list<std::unique_ptr<VisualBase>> m_NodeVisuals;
	unsigned int m_DnaScreenId = 0; // dna_screenのID(生成されたタイミングでセットする)
};