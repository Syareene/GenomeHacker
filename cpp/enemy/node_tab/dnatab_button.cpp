#include "enemy/node_tab/dnatab_button.h"
#include "manager.h"
#include "scene/game_scene.h"

void DNAButton::ToDNATab()
{
	// んー、こいつらって実態としてもたせるのかどうかを忘れてしもた
	// 実態としてもたせるならそもそもゲームシーン側のボタンの見た目とdna側での見た目と違うからややこしいような。

	// DNAタブに移動する処理
	
	// 該当タブの可視性を有効にし、game_sceneのstateをtabにする
	Scene* gameScene = Manager::GetCurrentScene().get();
	GameScene* scene_ptr = dynamic_cast<GameScene*>(gameScene);
	if (scene_ptr == nullptr)
	{
		// game_sceneではないのでreturn
		return;
	}

	scene_ptr->SetState(GameScene::State::DNA_TAB); // ゲームシーンの状態をDNAタブに変更

}

void DNAButton::ToGame()
{
	// ゲームに戻る処理
	
	// dnaタブの可視性を無効にし、game_sceneのstateをnormalにする
}