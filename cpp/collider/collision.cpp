#include "main.h"
#include "collider/collision.h"

void Collision::Update(const Vector3& obj_pos)
{
	// オブジェクトの位置にオフセットを加えた位置に更新
	SetPosition(obj_pos + m_PositionOffset);
	// 当たっているかどうかのフラグをリセット
	//m_IsHit = false;
}

void Collision::DrawCollider()
{
	// デバッグ用の衝突判定の可視化
	// ここで描画する
	// 例えばAABBなら箱を描画、Sphereなら球を描画
	// んーとりあえずAABBとSphereだけでいいかな?
	// OBBは回転が絡むからちょいめんどいし
}