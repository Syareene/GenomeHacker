#pragma once

#include "object/3d_object.h"

class Particle : public Object3D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	struct ParticleData
	{
		bool Enable;
		Vector3 Position;
		Vector3 Velocity;
		int LifeTime; // パーティクルの寿命
	};
	static constexpr int MaxParticles = 10000; // 最大パーティクル数
	ParticleData m_Particles[MaxParticles]; // パーティクルデータ
};