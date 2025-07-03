#pragma once

#include "object/gameObject.h"

class Particle : public GameObject
{
public:
	void Init() override;
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