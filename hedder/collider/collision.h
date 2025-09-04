#pragma once
#include "lib/vector3.h"

class Collision
{
	// Gameobjectにアタッチする用のプロパティ
public:
	void DrawCollider() const; // デバッグ用の衝突判定の可視化
protected:
	virtual void SetSphereProperty(const Vector3& center, float radius);
	virtual void SetAABBProperty(const Vector3& center, const Vector3& size);
	virtual void SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation);
private:
	// 衝突判定のプロパティ
	Vector3 m_Center; // 中心位置
	Vector3 m_Size; // サイズ(AABBとOBB用)
	Vector3 m_Rotation; // 回転値(OBB用)
	float m_Radius; // Sphereの半径

	// collisiontypeを判定するためにこれをoverrideして使ったほうがいいかなぁって気がしなくもない。
};