#pragma once

#include "object/game_object.h"

// 識別用のクラスに一旦はなりそう
class Object3D : public GameObject
{
public:
	// エラー解消用にコメントアウト
	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
};