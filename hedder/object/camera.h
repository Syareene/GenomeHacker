#pragma once

#include "3d_object.h"
#include "object/movement/title_cam.h"

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
	float m_ShakeTime = 0.0f;
	Vector3 m_ShakeVector = { 0.0f, 0.0f, 0.0f };
	std::unique_ptr<TitleCam> m_TitleCam;
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	inline void SetTitleCam()
	{
		m_TitleCam = std::make_unique<TitleCam>();
		m_TitleCam->Attach(this, Vector3(0.0f, 3.0f, -5.0f));
	}
	inline TitleCam* GetTitleCam() const { return m_TitleCam.get(); }
	void Shake(const Vector3& power);
	bool CheckView(const Vector3& position, const float radius);
	inline const Vector3 GetTarget() const { return m_Target; }
	inline void SetTarget(const Vector3& target) { m_Target = target; }
	inline XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	inline XMMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }
};