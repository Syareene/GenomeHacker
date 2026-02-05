#pragma once

#include "3d_object.h"
#include "object/movement/title_cam.h"

class Camera : public Object3D
{
private:
	constexpr static Vector3 DEFAULT_POSITION = Vector3(0.0f, 1.0f, -5.0f);
	constexpr static Vector3 DEFAULT_TARGET_OFFSET = Vector3(0.0f, 1.5f, 0.0f);
	constexpr static float ROTATION_VALUE = 2.0f;
	constexpr static float CULLING_RANGE = 30.0f; // カリング判定範囲
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
	Camera() = default; // デフォルトコンストラクタ
	virtual ~Camera() {};
	Camera(Camera&&) noexcept = default; // ムーブコンストラクタ
	Camera& operator=(Camera&&) noexcept = default; // ムーブ代入演算子

	static constexpr size_t MAX_OBJECTS = 1; // オブジェクトvector最大数。

	void Init(Transform trans = Transform());
	void Uninit() override;
	void Update() override;
	void Draw() override;
	inline void SetTitleCam()
	{
		m_TitleCam = std::make_unique<TitleCam>();
		m_TitleCam->Attach(this->GetObjectID(), Vector3(0.0f, 3.0f, -5.0f));
	}
	inline TitleCam* GetTitleCam() const { return m_TitleCam.get(); }
	void Shake(const Vector3& power);
	bool CheckView(const Vector3& position, const float radius);
	inline const Vector3 GetTarget() const { return m_Target; }
	inline void SetTarget(const Vector3& target) { m_Target = target; }
	inline XMMATRIX GetViewMatrix() const { return m_ViewMatrix; }
	inline XMMATRIX GetProjectionMatrix() const { return m_ProjectionMatrix; }
};