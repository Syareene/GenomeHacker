#pragma once

#include "object/gameObject.h"

class Player : public GameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	class ModelRenderer* m_ModelRenderer;
};