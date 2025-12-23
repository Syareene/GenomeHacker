#pragma once

#include "hedder/object/game_object.h"

class TitleCam
{
public:
	void Attach(GameObject* pointer, const Vector3& start_pos);
	void Detach();
	void Move(const float& speed);
private:
	GameObject* m_AttachObject = nullptr;
	std::vector<std::pair<Vector3, float>> m_MovementList; // 移動リスト(移動点、必要時間)
	float m_Count = 0.0f;
	int m_CurrentIndex = 0;
};