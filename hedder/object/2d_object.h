#pragma once

#include "object/game_object.h"


// 識別用のクラスに一旦はなりそう
class Object2D : public GameObject
{
public:
	Object2D() = default; // デフォルトコンストラクタ
	virtual ~Object2D() {}
	Object2D(Object2D&&) noexcept = default; // ムーブコンストラクタ
	Object2D& operator=(Object2D&&) noexcept = default; // ムーブ代入演算子
	void Init(Transform trans = Transform()) override
	{
		SetTransform(trans);
	};
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};