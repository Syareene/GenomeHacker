#pragma once

#include <list>
#include <memory>
#include <string>
#include "object/game_object.h"
#include "object/system_object.h"
#include "object/3d_object.h"
#include "object/2d_object.h"

class Object3D; // 前方宣言


class State : public Scene
{
public:
	State() = default;
	State(State&& other) noexcept = default;
	// 
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// このstate内限定で管理するオブジェクトを追加したり変更する関数群
	// んー、管理としてはこっちのほうが見やすいが、すでにあるscene側の配列に対して追加でstate指定をしてあげて
	// それを元に実行時にフィルターするほうが(なおソートをする必要があるので実行は遅くなるかな)
	
	// updatebytag,tags,drawbytag,tagsをstate内管理限定版を作成
	// addgame/systemobj、object取得系もstate内管理限定版を作成

};