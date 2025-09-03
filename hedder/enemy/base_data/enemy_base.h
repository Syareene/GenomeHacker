#pragma once

#include <list>
#include <memory>
#include "enemy/dna_screen_script.h"
#include "object/ui/button.h"


class EnemyBase
{
public:
	virtual void Register(); // 登録処理
	void Unregister(); // 登録解除処理
	// ノードの内容を下に行動させる関数(攻撃、動き、死亡時)
	void ExecuteAttack();
	void ExecuteMove();
	bool ExecuteDeath();

	// 敵リストタブ->DNAタブに遷移するボタンを表示するための関数->これボタンインスタンスを生成しないといけないから構造については考える必要あり
	// ただ関数としてはここにほしいかな
	void ShowToDnaButton(Vector2 pos, const std::wstring filePath);
	void HideToDnaButton();
	// インスタンスを都度生成すると重たすぎるが保持しすぎてもメモリを圧迫するっていう
	// 一旦都度生成でいく

	// DNAタブ関連の関数
	void ShowDnaScreen(); // 呼ばれたらコイツ自身のnode情報を持っているscriptを表示
	void HideDnaScreen();

	void SetEnemyID(int id) { m_EnemyID = id; }
	int GetEnemyID() const { return m_EnemyID; }

	int SetTextureID(const std::wstring filePath, std::pair<int, int> texTarget = {0, 0}, std::pair<int, int> texCount = {1, 1});
	int GetEnemyTextureID() const { return m_TextureID; }

	// setはとりあえずglobalに。今は使わないかもだけど後々scaleに応じて体力設定とかしたいなら使う。
	void SetMaxHealth(float maxHealth) { m_MaxHealth = maxHealth; }
	float GetMaxHealth() const { return m_MaxHealth; }
private:
	std::unique_ptr<DnaScreenScript> m_DnaScreen; // dnaタブをまとめているpanel->初期化時に自身のnodeを持つために作成する必要あり
	Button* m_ToDnaButton = nullptr; // 生成したボタンオブジェクトのポインタ。scene側に保持している物のポインタとなる。消すときはここから取得したのに対してdestoryを設定すれば良い
	// static -> ボタン押すときの外枠テクスチャ用変数?
	// 敵自体のテクスチャ?ただその場合テクスチャ元とuvを両方保存しないといけない、、
	int m_TextureID = -1; // 敵のテクスチャID
	int m_EnemyID = -1; // 敵のID
	std::pair<int, int> m_TextureTarget{ 0, 0 }; // 対象となるテクスチャの場所(幅、高さ)
	std::pair<int, int> m_TextureCount{ 1, 1 }; // テクスチャの分割数(横、縦) -> これでuvを計算する

	// enemy共通で見るnode以外のステータスを格納する。
	float m_MaxHealth; // 最大体力

};