// 基礎データからデータを持ってきてフィールドに描画する
// データを読み取ってから書くのでこのクラスは継承されずデータを受け取り挙動を変えるクラスとなる

#include "enemy/field_enemy.h"

void FieldEnemy::Init()
{
	// 初期化処理
	Object3D::Init();
	// テクスチャは敵データから描画時に取得するのでいらない
}

void FieldEnemy::Uninit()
{
	// 終了処理
	Object3D::Uninit();
}

void FieldEnemy::Update()
{
	// 更新処理
	Object3D::Update();
	
}

void FieldEnemy::Draw()
{
	// 描画処理
	Object3D::Draw();
	
}