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

	std::vector<NodeBase>& GetAllNodes()
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
		return &(*it);
	}
	std::vector<VisualBase>& GetAllVisualNodes()
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
		return &(*it);
	}
	// この関数引数&なのと、init入れるかどうか
	void AddVisualNode(const VisualBase& visual)
	{
		m_NodeVisuals.push_back(visual);
	}
private:
	class ModelRenderer* m_ModelRenderer = nullptr;
	std::vector<NodeBase>m_HavingNodes;
	std::vector<VisualBase> m_NodeVisuals;
	unsigned int m_DnaScreenId = 0; // dna_screenのID(生成されたタイミングでセットする)
};