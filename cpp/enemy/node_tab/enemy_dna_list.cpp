#include "main.h"
#include "enemy/node_tab/enemy_dna_list.h"
#include "enemy/base_data/enemy_list.h"
#include "enemy/base_data/enemy_base.h"
#include "scene/manager.h"
#include "lib/mouse.h"
#include "scene/game_scene.h"
#include "scene/state/dna_table_state.h" // for type checks

void EnemyDnaList::Init()
{
	AddTag("system");
}

void EnemyDnaList::Uninit()
{

}

void EnemyDnaList::Update()
{
	// enemy_baseを元に再起し個別にノードいじる場所に整列

	// stateが変わっているかをチェック
	Scene* temp = Manager::GetCurrentScene().get();
	GameScene* scene = dynamic_cast<GameScene*>(temp);

	if(scene == nullptr)
	{
		// game_sceneではないのでreturn
		assert(false);
		return;
	}

	// 敵一覧を取得
	std::list<std::unique_ptr<EnemyBase>>& enemy_base_list = Manager::GetCurrentScene()->GetSystemObject<EnemyList>()->GetEnemyBases();

	// stateが変わっており、かつstateがdna_tabでない場合にボタンを消す
	if (scene->IsStateChanged() && !scene->IsState<DnaTableState>())
	{
		// ボタン消す
		for (auto& enemyBase : enemy_base_list)
		{
			enemyBase->HideDnaEditButton();
		}
		// 消したのでreturn
		return;
	}

	// dna_tabでない場合はreturn
	if(!scene->IsState<DnaTableState>())
	{
		return;
	}

	// GameSceneの場合、stateが変わっており、かつstateがdna_tabの場合にボタンを生成
	if (scene->IsStateChanged() && scene->IsState<DnaTableState>())
	{
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

	// スクロール対応
	int scroll_value = Mouse::GetWheelDiff();


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