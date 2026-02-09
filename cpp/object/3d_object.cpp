#include "main.h"
#include "object/3d_object.h"
#include "collider/collision.h"

// Object3D のデストラクタをcppで定義
Object3D::~Object3D() = default;
Object3D::Object3D(Object3D&& other) noexcept = default;
Object3D& Object3D::operator=(Object3D&& other) = default;