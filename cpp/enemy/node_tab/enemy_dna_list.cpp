#include "main.h"
#include "enemy/node_tab/enemy_dna_list.h"
#include "enemy/base_data/enemy_list.h"
#include "enemy/base_data/enemy_base.h"
#include "scene/manager.h"
#include "lib/mouse.h"
#include "scene/game_scene.h"
#include "scene/state/dna_table_state.h"

void EnemyDnaList::Init()
{
	AddTag("system");

	// 敵一覧を取得
	std::list<std::unique_ptr<EnemyBase>>& enemy_base_list = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases();

	// 整列用カウント変数
	int width_count = 0;
	int height_count = 0;

	for (auto& enemyBase : enemy_base_list)
	{
		enemyBase->ShowDnaEditButton(Vector2(200.0f + (width_count * (WIDTH_SIZE + WIDTH_SIZE / 8.0f)), 300.0f + (height_count * (HEIGHT_SIZE + HEIGHT_SIZE / 12.0f))), Vector2(WIDTH_SIZE, HEIGHT_SIZE), enemyBase->GetEnemyTextureID());
		// カウント更新
		width_count++;
		if (width_count >= COLUMN_NUM)
		{
			width_count = 0;
			height_count++;
		}
	}
}

void EnemyDnaList::Uninit()
{
	// 敵一覧を取得
	std::list<std::unique_ptr<EnemyBase>>& enemy_base_list = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases();

	for(auto& enemyBase : enemy_base_list)
	{
		enemyBase->HideDnaEditButton();
	}
}

void EnemyDnaList::Update()
{
	// スクロール対応
	int scroll_value = Mouse::GetWheelDiff();

	// 敵一覧を取得
	std::list<std::unique_ptr<EnemyBase>>& enemy_base_list = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases();


	// 整列用カウント変数
	int width_count = 0;
	int height_count = 0;


	for(auto& enemyBase : enemy_base_list)
	{
		// スクロール量に応じて座標を更新。
		// これ以上スクロールされてほしくない領域に来たらテクスチャ座標を変えなければならぬ。
		// 横の奴らは頂点データ同じの利用したいよね、、

		// カウント更新
		width_count++;
		if(width_count >= COLUMN_NUM)
		{
			width_count = 0;
			height_count++;
		}
	}	
}


void EnemyDnaList::UpdateFinal()
{
	
}
