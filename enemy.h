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
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	class ModelRenderer* m_ModelRenderer;
};