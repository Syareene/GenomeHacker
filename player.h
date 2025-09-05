#pragma once

#include "object/3d_object.h"

class Player : public Object3D
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	class ModelRenderer* m_ModelRenderer = nullptr;
};