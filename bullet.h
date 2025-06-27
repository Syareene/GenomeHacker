#pragma once

#include "object/gameObject.h"

class Bullet : public GameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	Vector3 GetVelocity() const { return m_Velocity; }
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer;
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	int m_LifeTime = 120;
};