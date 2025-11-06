#pragma once


#include <numbers>

class Transform
{
private:
	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;
	Vector3 m_Radian;
	// 回転角をラジアンに変換する関数
	inline void ConvertRotationToRadian()
	{
		m_Radian.x = m_Rotation.x * std::numbers::pi_v<float> / 180.0f;
		m_Radian.y = m_Rotation.y * std::numbers::pi_v<float> / 180.0f;
		m_Radian.z = m_Rotation.z * std::numbers::pi_v<float> / 180.0f;
	}
public:
	Transform()
	{
		// 初期値をセット
		m_Position = Vector3(0.0f, 0.0f, 0.0f);
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
		m_Radian = Vector3(0.0f, 0.0f, 0.0f);
	};
	inline const Vector3& GetPosition() const { return m_Position; }
	inline void SetPosition(const Vector3& pos) { m_Position = pos; }
	inline const Vector3& GetScale() const { return m_Scale; }
	inline void SetScale(const Vector3& scale) { m_Scale = scale; }
	inline const Vector3& GetRotation() const { return m_Rotation; }
	inline void SetRotation(const Vector3& rot)
	{ 
		m_Rotation = rot; 
		// ラジアン角も保存
		ConvertRotationToRadian();
	}
	inline const Vector3& GetRadian() const { return m_Radian; }
};