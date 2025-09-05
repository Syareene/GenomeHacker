#pragma once

#include "object/game_object.h"
#include <memory>
#include "collider/collision.h"

//class Collision;

// 識別用のクラスに一旦はなりそう
class Object3D : public GameObject
{
public:
	// エラー解消用にコメントアウト
	void Init() override {}
	void Uninit() override {}
	void Update() override {}
	void Draw() override {}
	template <typename T>
	T* SetCollider()
	{
		/*
		if (std::is_base_of_v<Collision, T>)
		{
			m_Collider = std::make_unique<T>();
		}
		*/

		m_Collider = std::make_unique<T>();
		return dynamic_cast<T*>(m_Collider.get());
	};
	Collision* GetCollider() const { return m_Collider.get(); }
private:
	std::unique_ptr<Collision> m_Collider = nullptr; // 球か箱かのポインタを持つ
};