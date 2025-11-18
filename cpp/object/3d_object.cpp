#include "main.h"
#include "object/3d_object.h"
#include "collider/collision.h"

// Object3D のデストラクタを翻訳単位内で定義（unique_ptr<Collision> の delete を正しく生成させるため）
Object3D::~Object3D() = default;