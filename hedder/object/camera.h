#pragma once

#include "3d_object.h"

class Camera : public Object3D
{
private:
	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
	float m_NearZ = 0.0f;
	float m_FarZ = 0.0f;
	float m_FieldOfViewY = 0.0f;
	float m_AspectRatio = 0.0f;
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	Vector3 GetTarget() const { return m_Target; }
	void SetTarget(const Vector3& target) { m_Target = target; }
	XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	XMMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }
};