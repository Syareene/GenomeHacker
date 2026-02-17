#pragma once

#include "3d_object.h"

class AreaObject : public Object3D
{
public:
	static void SetPipelineState();
	void UpdateGPUData(InstanceBufferData& data) override;
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetMaxDuration(const int& duration) { m_MaxDuration = duration; }
	int GetMaxDuration() const { return m_MaxDuration; }
	void ResetDuration() { m_Duration = 0; }
	int GetCurrentDuration() const { return m_Duration; }
private:
	int m_MaxDuration = 0; // エリアの最大持続時間(フレーム数)
	int m_Duration = 0;
};