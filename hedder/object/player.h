#pragma once

#include "object/3d_object.h"
#include "enemy/node/base.h"
#include "enemy/node/visual_base.h"

class Player : public Object3D
{
public:
	static void SetPipelineState();
	void UpdateGPUData(InstanceBufferData& data) override;
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
	// 見た目ノード追加
	void AddVisualNode(int index, NodeBase* nodePtr)
	{
		m_NodeVisuals.emplace_back(std::make_unique<VisualBase>());
		m_NodeVisuals.back()->Init(m_DnaScreenId, index, nodePtr);
	}
private:
	constexpr static Vector3 MODEL_SCALE = Vector3(1.5f, 1.5f, 1.0f);
	constexpr static Vector3 COLLIDER_SCALE = Vector3(0.5f, 0.5f, 0.5f);
	constexpr static float PLAYER_MOVE_SPEED = 0.1f;
	constexpr static float BULLET_SPAWN_OFFSET_Y = 1.0f;
	constexpr static float BULLET_MOVE_SPEED = 0.15f;
	class ModelRenderer* m_ModelRenderer = nullptr;
	std::list<std::unique_ptr<NodeBase>>m_HavingNodes;
	std::list<std::unique_ptr<VisualBase>> m_NodeVisuals;
	unsigned int m_DnaScreenId = 0; // dna_screenのID(生成されたタイミングでセットする)
};