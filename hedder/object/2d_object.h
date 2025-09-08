#pragma once

#include "object/game_object.h"


// 識別用のクラスに一旦はなりそう
class Object2D : public GameObject
{
public:
	virtual ~Object2D() { OutputDebugStringA("~Object2D\n"); }
	// エラー解消用にコメントアウト
	void Init(Transform trans = Transform()) override
	{
		SetTransform(trans);
	};
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};