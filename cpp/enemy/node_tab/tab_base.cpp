#include "enemy/node_tab/tab_base.h"

void TabBase::Init(Transform trans)
{
	// タブの初期化処理
	Button::Init();
	
}

void TabBase::Uninit()
{
	// タブの終了処理
	Button::Uninit();
	
}

void TabBase::Update()
{
	// タブの更新処理
	Button::Update();
	// タブ内にあるノードに対する更新処理
}

void TabBase::Draw()
{
	// タブの描画処理
	Button::Draw();
	// タブ内にあるノードに対する描画処理

	// 自身の描画
}

void TabBase::Clicked()
{
	// クリックされたときの処理
	// 例えばタブの切り替えとか
}