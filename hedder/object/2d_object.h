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
	void Init(Transform trans = Transform())
	{
		SetTransform(trans);
	};
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
	template <typename T>
	static int getTypeId()
	{
		static int id = nextTypeId();
		return id;
	}
private:
	static int nextTypeId()
	{
		static int id = 0;
		return id++;
	}
};