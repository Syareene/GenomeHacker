#pragma once

#include "object/game_object.h"


// こーれパネルの使用どうしよ。2d限定とか?
class Panel : public GameObject
{
private:
	// 描画順だけどうするかだねぇ各オブジェクトの
	std::list<GameObject> m_ChildObjects; // 子オブジェクトのリスト
};