#pragma once
#include "collider/collision.h"

#include <vector>


struct ID3D11Buffer; // 前方宣言
struct VERTEX_3D; // 前方宣言

class Sphere : public Collision
{
public:
	void Init(const Transform& trans = Transform(), const Vector3& pos_diff = { 0.0f, 0.0f, 0.0f }) override;
	void Uninit() override;
	void Update(const Vector3& obj_pos) override;
	void DrawCollider() override;

	// コリジョン取得
	bool CheckCollision(const Collision& other) override;
	bool CheckCollisionSphere(const Collision& other) const override;
	bool CheckCollisionAABB(const Collision& other) const override;
	bool CheckCollisionOBB(const Collision& other) const override;
private:
	constexpr static int m_CircleVertexCount = 12; // 円を描く頂点数
	void MakeCircleVertex(int vertex_count, std::vector<Vector3>& outVertex);
	static ID3D11Buffer* m_VertexBuffer; // 頂点バッファ

	// 設定したいプロパティとかは基底クラスにあるから一旦形だけ?

	// Sphere-Sphere
	// Sphere-AABB
	// Sphere-OBB
};