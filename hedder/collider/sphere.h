#pragma once
#include "collider/collision.h"
#include "lib/vector3.h"
#include <vector>


struct ID3D11Buffer; // 前方宣言
struct VERTEX_3D; // 前方宣言

class Sphere : public Collision
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void SetSphereProperty(const Vector3& center, float radius) override
	{
		// 衝突判定のプロパティ設定
		Collision::SetSphereProperty(center, radius);
	}

	void DrawCollider() override;
private:
	constexpr static int m_CircleVertexCount = 24; // 円を描く頂点数
	void MakeCircleVertex(int vertex_count, std::vector<VERTEX_3D>& outVertex);
	static ID3D11Buffer* m_VertexBuffer; // 頂点バッファ

	// 設定したいプロパティとかは基底クラスにあるから一旦形だけ?

	// Sphere-Sphere
	// Sphere-AABB
	// Sphere-OBB
};