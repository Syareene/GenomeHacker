#pragma once

#include "object/2d_object.h"

// まだ汎用化してないので機能を切り出して汎用化する必要あり


class Particle2D : public Object2D
{
public:
	void Register(int maxCount);
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void UpdateParticle();
	void Draw() override;

	// 1. パーティクル単体のデータ (CPU計算用)
	struct ParticleData
	{
		Vector2 Position;
		Vector2 Velocity;
		XMFLOAT4 Color;
		float LifeTime;      // 残り生存時間
		float MaxLifeTime;   // 最大生存時間（進行度の計算用）
		float Size;
		bool IsActive;
	};

	// 2. GPUに送るインスタンスデータ (定数バッファや頂点バッファ用)
	struct InstanceData 
	{
		XMFLOAT4 PositionAndSize; // xy: Position, z: Size, w: Rotation
		XMFLOAT4 Color;
		XMFLOAT4 UVOffset;        // テクスチャアトラス用 (xy: offset, zw: scale)
	};

    // 振る舞いをセットする関数（これで動きや初期化を自由に差し替え可能）
    void SetInitBehavior(std::function<void(ParticleData&)> func) 
	{
        initBehavior = func;
    }

    void SetUpdateBehavior(std::function<void(ParticleData&, float)> func) 
	{
        updateBehavior = func;
    }

    // パーティクル発生
    void Emit(int count, Vector2 startPos, float size, XMFLOAT4 color) 
	{
        int emitted = 0;
        for (auto& p : m_Particles) 
		{
            if (!p.IsActive) 
			{
                p.IsActive = true;
                p.Position = startPos;
                p.LifeTime = 750.0f; // デフォルト
				p.Size = size;
				p.Color = color;

                // 設定された初期化ロジックを実行
				if (initBehavior)
				{
					initBehavior(p);
				}

                emitted++;
				if (emitted >= count)
				{
					break;
				}
            }
        }
    }

private:
	int m_Count = 0;
	ID3D11Buffer* m_InstanceBuffer = nullptr;
	ID3D11ShaderResourceView* m_InstanceSRV = nullptr;
	// プールされたパーティクル配列
	std::vector<ParticleData> m_Particles;
	// GPU転送用バッファ（毎フレーム書き換える）
	std::vector<InstanceData> m_InstanceDataBuffer;

	// 設定パラメータ
	int maxParticles;
	int activeParticleCount = 0;

	// 振る舞いを関数オブジェクトとして保持
	// 初期化時の振る舞い（ランダムな拡散など）
	std::function<void(ParticleData&)> initBehavior;
	// 更新時の振る舞い（重力、減速、色の変化など）
	std::function<void(ParticleData&, float)> updateBehavior;
};