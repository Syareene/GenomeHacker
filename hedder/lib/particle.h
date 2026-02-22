#pragma once

#include "object/3d_object.h"

// これ授業内でやったのをそのまま使ってるのでより最適化の必要あるかも

class Particle : public Object3D
{
public:
	void Init(Transform trans = Transform());
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
	constexpr static int MaxParticles = 10000; // 最大パーティクル数
	ParticleData m_Particles[MaxParticles]; // パーティクルデータ
private:
	constexpr static Vector3 PARTICLE_SCALE = Vector3(0.3f, 0.3f, 0.3f);
	constexpr static int PARTICLE_LIFETIME = 60; // パーティクルの寿命(フレーム)
	constexpr static float VELOCITY_RANDOM_RANGE = 1.0f;
};