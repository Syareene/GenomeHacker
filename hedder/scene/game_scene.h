#pragma once

#include "base_scene.h"
#include "enemy/enemy_spawner.h"

class GameScene : public Scene
{
public:
	enum class State
	{
		NORMAL, // 通常時
		ESC_MENU, // escメニュー表示時
		DNA_TAB, // DNAタブ表示時
	};
private:
	class Audio* m_BGM;
   State m_State = State::NORMAL; // 初期状態は通常時
   State m_WillState = State::NORMAL; // 次の状態を保持する変数(状態切り替え時に使用)
   std::unique_ptr<EnemySpawner> m_EnemySpawner = nullptr; // 敵のスポーンを管理するクラス(gameObjectではないのでこっちで管理する)
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetState(State state) { m_WillState = state; } // 状態を設定する関数->これシーンの最後に状態切り替えるような処理にしておくこと
	State GetState() const { return m_State; } // 現在の状態を取得する関数
};