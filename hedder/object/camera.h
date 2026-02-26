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
	// ムーブコンストラクタ
	Camera(Camera&& other) noexcept
		: Object3D(std::move(other))
		, m_Target(std::move(other.m_Target))
		, m_ViewMatrix(std::move(other.m_ViewMatrix))
		, m_ProjectionMatrix(std::move(other.m_ProjectionMatrix))
		, m_NearZ(other.m_NearZ)
		, m_FarZ(other.m_FarZ)
		, m_FieldOfViewY(other.m_FieldOfViewY)
		, m_AspectRatio(other.m_AspectRatio)
		, m_ShakeTime(other.m_ShakeTime)
		, m_ShakeVector(std::move(other.m_ShakeVector))
		, m_TitleCam(std::move(other.m_TitleCam))
	{
	}

	// ムーブ代入演算子
	Camera& operator=(Camera&& Other) noexcept
	{
		if (this != &Other)
		{
			Object3D::operator=(std::move(Other));
			m_Target = std::move(Other.m_Target);
			m_ViewMatrix = std::move(Other.m_ViewMatrix);
			m_ProjectionMatrix = std::move(Other.m_ProjectionMatrix);
			m_NearZ = Other.m_NearZ;
			m_FarZ = Other.m_FarZ;
			m_FieldOfViewY = Other.m_FieldOfViewY;
			m_AspectRatio = Other.m_AspectRatio;
			m_ShakeTime = Other.m_ShakeTime;
			m_ShakeVector = std::move(Other.m_ShakeVector);
			m_TitleCam = std::move(Other.m_TitleCam);
		}
		return *this;
	}

	static constexpr size_t MAX_OBJECTS = 1; // オブジェクトvector最大数。
	static constexpr bool ENABLE_INSTANCING = false; // インスタンスレンダリング無効

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