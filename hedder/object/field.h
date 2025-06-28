#pragma once

#include "gameObject.h"

class Field : public GameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
};