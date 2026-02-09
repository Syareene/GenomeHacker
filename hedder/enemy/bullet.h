#pragma once

#include "object/3d_object.h"

class Bullet : public Object3D
{
public:
	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	const Vector3 GetVelocity() const { return m_Velocity; }
private:
	constexpr static Vector3 POSITION_OFFSET = Vector3(0.0f, -1.0f, 0.0f);
	constexpr static Vector3 SCALE = Vector3(0.225f, 0.225f, 0.225f);

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class ModelRenderer* m_ModelRenderer = nullptr;
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	int m_LifeTime = 120;
};