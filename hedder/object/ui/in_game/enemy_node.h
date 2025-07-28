#pragma once

#include "object/panel.h"


// 一旦仮作成
class EnemyNode : public Panel
{
	// 内包するオブジェクトはinit時に代入する感じに。
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override; // 描画処理
	void Activate();
private:
	void Deactivate();
};