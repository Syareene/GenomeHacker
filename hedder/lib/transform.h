#pragma once

#include "lib/vector3.h"

class Transform
{
private:
	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;
public:
	Transform()
	{
		// 初期値をセット
		m_Position = Vector3(0.0f, 0.0f, 0.0f);
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	};
	const Vector3& GetPosition() const { return m_Position; }
	void SetPosition(const Vector3& pos) { m_Position = pos; }
	const Vector3& GetScale() const { return m_Scale; }
	void SetScale(const Vector3& scale) { m_Scale = scale; }
	const Vector3& GetRotation() const { return m_Rotation; }
	void SetRotation(const Vector3& rot) { m_Rotation = rot; }
};