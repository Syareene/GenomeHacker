#include "collider/collision.h"

void Collision::DrawCollider()
{
	// デバッグ用の衝突判定の可視化
	// ここで描画する
	// 例えばAABBなら箱を描画、Sphereなら球を描画
	// んーとりあえずAABBとSphereだけでいいかな?
	// OBBは回転が絡むからちょいめんどいし
}

void Collision::SetSphereProperty(const Vector3& center, const Vector3& size)
{
	// 衝突判定のプロパティ設定
	m_Center = center;
	m_Scale = size;
}

void Collision::SetAABBProperty(const Vector3& center, const Vector3& size)
{
	// 衝突判定のプロパティ設定
	m_Center = center;
	m_Scale = size;
}

void Collision::SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation)
{
	// 衝突判定のプロパティ設定
	m_Center = center;
	m_Scale = size;
	m_Rotation = rotation;
}