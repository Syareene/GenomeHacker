#pragma once

#include "hedder/object/game_object.h"

class TitleCam
{
public:
	TitleCam() = default;
	virtual ~TitleCam() {}
	// ムーブコンストラクタ
	TitleCam(TitleCam&& Other) noexcept
		: m_AttachObjectID(Other.m_AttachObjectID)
		, m_MovementList(std::move(Other.m_MovementList))
		, m_Count(Other.m_Count)
		, m_CurrentIndex(Other.m_CurrentIndex)
	{
	}
	// ムーブ代入演算子
	TitleCam& operator=(TitleCam&& Other) noexcept
	{
		if (this != &Other)
		{
			m_AttachObjectID = Other.m_AttachObjectID;
			m_MovementList = std::move(Other.m_MovementList);
			m_Count = Other.m_Count;
			m_CurrentIndex = Other.m_CurrentIndex;
		}
		return *this;
	}


	void Attach(unsigned int objId, const Vector3& start_pos);
	void Detach();
	void Move(const float& speed);
private:
	unsigned int m_AttachObjectID = 0;
	std::vector<std::pair<Vector3, float>> m_MovementList; // 移動リスト(移動点、必要時間)
	float m_Count = 0.0f;
	int m_CurrentIndex = 0;
};