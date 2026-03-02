#include "main.h"
#include "object/2d_object.h"


// デストラクタ
Object2D::~Object2D() = default;

// ムーブコンストラクタ
Object2D::Object2D(Object2D&& other) noexcept
	: GameObject(std::move(other))
{
}

// ムーブ代入演算子
Object2D& Object2D::operator=(Object2D&& other) noexcept
{
	if (this != &other)
	{
		GameObject::operator=(std::move(other));
	}
	return *this;
}