#pragma once

class EnemySpawner
{
	// 基本はgameSceneにおいて、呼び出したときにオブジェクトを生成するだけのクラス
	// 今後機能が拡張するとウェーブ管理とかもするので敵の召喚をgameScene側から関数を通じて指定するのではなく
	// こっち側で判断するように
public:
	void Init();
	void Uninit();
	void Update();
private:
	EnemySpawner() = default; // newできないように->この場合は自身でインスタンスの所在を持ってないと行けなくはなるけどね。一旦検討
	void SpawnEnemy();
	// 敵の元データを格納する変数
	// ポインタを受け取って特定の敵の初期化情報をセットする関数
};