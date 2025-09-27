#include "enemy/node_tab/enemy_dna_list.h"
#include "enemy/base_data/enemy_list.h"
#include "scene/manager.h"
#include "lib/mouse.h"

void EnemyDnaList::Init()
{

}

void EnemyDnaList::Uninit()
{

}

void EnemyDnaList::Update()
{
	// enemy_baseを元に再起し個別にノードいじる場所に整列


	/*
	std::list<std::unique_ptr<EnemyBase>> enemy_base_list = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases();

	// スクロール対応
	int scroll_value = Mouse::GetWheelDiff();


	// 整列用カウント変数
	int width_count = 0;
	int height_count = 0;


	for(auto & enemyBase : enemy_base_list)
	{
		// ここでenemyBaseのdnaScreenを表示する
		enemyBase->ShowDnaEditButton(Vector2(200.0f + (width_count * (WIDTH_SIZE + WIDTH_SIZE / 8.0f)), (height_count * (HEIGHT_SIZE + HEIGHT_SIZE / 12.0f))), Vector2(WIDTH_SIZE, HEIGHT_SIZE), enemyBase->GetEnemyTextureID());
		// カウント更新
		width_count++;
		if(width_count >= COLUMN_NUM)
		{
			width_count = 0;
			height_count++;
		}
	}

	*/
}