#pragma once

#include "object/3d_object.h"

class Enemy : public Object3D
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer = nullptr;
};