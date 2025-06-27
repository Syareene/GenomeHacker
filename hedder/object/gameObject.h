#pragma once

#include "vector3.h"
#include "main.h" // あんまりこれで読み込みたくないんだよな、、
#include <string>

class GameObject
{
private:
	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };
	bool m_Destory = false; // 削除予約フラグ(今は別の方法で検知している為使っていない)
	int m_TextureID = -1;
	std::string m_Tag; // タグを設定してグループで判定できるように

public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	Vector3& GetPosition() { return m_Position; }
	void SetPosition(Vector3 Position) { m_Position = Position; }
	Vector3& GetRotation() { return m_Rotation; }
	void SetRotation(Vector3 Rotation) { m_Rotation = Rotation; }
	Vector3& GetScale() { return m_Scale; }
	void SetScale(Vector3 Scale) { m_Scale = Scale; }
	int GetTextureID() const { return m_TextureID; }
	void SetTextureID(int TextureID) { m_TextureID = TextureID; }
	std::string GetTag() const { return m_Tag; }
	void SetTag(const std::string& tag) { m_Tag = tag; }

	void SetDestory(bool Destory) { m_Destory = Destory; }
	bool IsDestory() const { return m_Destory; }
	bool Destory()
	{ 
		if (m_Destory)
		{
			Uninit();
			delete this;
			return true; // 削除予約フラグが立っていたらtrueを返す
		}
		return false;
	}

	Vector3 GetRight()
	{
		// 回転行列から右方向のベクトルを取得
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		// XMFLOAT3に変換してからVector3に変換
		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);
		return right;
	}
	Vector3 GetUp()
	{
		// 回転行列から上方向のベクトルを取得
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		// XMFLOAT3に変換してからVector3に変換
		Vector3 up;
		XMStoreFloat3((XMFLOAT3*)&up, matrix.r[1]);
		return up;
	}

	Vector3 GetForward()
	{
		// 回転行列から前方向のベクトルを取得
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		// XMFLOAT3に変換してからVector3に変換
		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);
		return forward;
	}

	float GetDistance(Vector3 Position)
	{
		return (Position - m_Position).length();
	}
};