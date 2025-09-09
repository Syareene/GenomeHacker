#pragma once

#include "object/3d_object.h"

class Bullet : public Object3D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	const Vector3 GetVelocity() const { return m_Velocity; }
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer = nullptr;
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	int m_LifeTime = 120;
};