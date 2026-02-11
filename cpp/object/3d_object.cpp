#include "main.h"
#include "object/3d_object.h"
#include "collider/collision.h"

// Object3D のデストラクタをcppで定義
Object3D::~Object3D() = default;
Object3D::Object3D(Object3D&& other) noexcept = default;
Object3D& Object3D::operator=(Object3D&& other) = default;

void Object3D::DrawCollider()
{
	// コライダが存在しない、アクティブでない、削除予約されている場合は何もしない
    if (!GetCollider() || !IsActive() || IsDestroy())
    {
        return;
    }

    // コライダの描画関数を呼ぶ
    GetCollider()->DrawCollider();
}