#pragma once

#include "object/gameObject.h"

class Panel : public GameObject
{
private:
	// 描画順だけどうするかだねぇ各オブジェクトの
	std::list<GameObject> m_ChildObjects; // 子オブジェクトのリスト
};