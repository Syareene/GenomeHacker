#include "main.h"
#include "object/3d_object.h"
#include "collider/collision.h"

// Object3D のデストラクタをcppで定義
Object3D::~Object3D() = default;

// ムーブコンストラクタ
Object3D::Object3D(Object3D&& other) noexcept
    : GameObject(std::move(other))
    , m_Collider(std::move(other.m_Collider))
{

}

// ムーブ代入演算子
Object3D& Object3D::operator=(Object3D&& other) noexcept
{
    if (this != &other)
    {
        GameObject::operator=(std::move(other));
        m_Collider = std::move(other.m_Collider);
    }
    return *this;
}

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