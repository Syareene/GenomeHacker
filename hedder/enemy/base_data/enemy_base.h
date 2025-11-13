#pragma once

#include <list>
#include <memory>
#include "enemy/dna_screen_script.h"
#include "object/ui/button.h"


class FieldEnemy; // 前方宣言


class EnemyBase
{
public:
	virtual void Register(); // 登録処理
	void Unregister(); // 登録解除処理
	// ノードの内容を下に行動させる関数(攻撃、動き、死亡時)
	void ExecuteAttack(FieldEnemy* enemy_ptr);
	void ExecuteMove(FieldEnemy* enemy_ptr);
	bool ExecuteDeath(FieldEnemy* enemy_ptr);

	// DnaScreenにあるオブジェクトを操作するための関数群
	void Init();
	void Uninit();
	void Update();
	void Draw();


	// 敵リストタブ->DNAタブに遷移するボタンを表示するための関数->これボタンインスタンスを生成しないといけないから構造については考える必要あり
	// ただ関数としてはここにほしいかな
	void ShowDnaEditButton(const Vector2& pos, const Vector2& size, const int texID);
	void HideDnaEditButton();
	// テクスチャid版がほしい


	// インスタンスを都度生成すると重たすぎるが保持しすぎてもメモリを圧迫するっていう
	// 一旦都度生成でいく

	// DNAタブ関連の関数
	//inline void SetDnaScreen(std::unique_ptr<DnaScreenScript> dnaScreen) { m_DnaScreen = std::move(dnaScreen); }
	inline DnaScreenScript* GetDnaScreen() {return m_DnaScreen; }
	void ShowDnaScreen(); // 呼ばれたらコイツ自身のnode情報を持っているscriptを表示
	void HideDnaScreen();

	inline void SetEnemyID(int id) { m_EnemyID = id; }
	inline int GetEnemyID() const { return m_EnemyID; }

	int SetTextureID(const std::wstring filePath, std::pair<int, int> texTarget = {0, 0}, std::pair<int, int> texCount = {1, 1});
	inline const int GetEnemyTextureID() const { return m_TextureID; }

	// setはとりあえずglobalに。今は使わないかもだけど後々scaleに応じて体力設定とかしたいなら使う。
	inline void SetMaxHealth(const float& maxHealth) { m_MaxHealth = maxHealth; }
	inline const float GetMaxHealth() const { return m_MaxHealth; }

	inline void SetDrawPosDiff(const Vector3& posDiff) { m_PosDiff = posDiff; }
	inline const Vector3& GetDrawPosDiff() const { return m_PosDiff; }
	inline void SetDrawScaleDiff(const Vector3& scaleDiff) {m_ScaleDiff = scaleDiff;}
	inline const Vector3& GetDrawScaleDiff() const { return m_ScaleDiff; }
private:
	DnaScreenScript* m_DnaScreen; // dnaタブをまとめているpanel->初期化時に自身のnodeを持つために作成する必要あり
	Button* m_ToDnaButton = nullptr; // 生成したボタンオブジェクトのポインタ。scene側に保持している物のポインタとなる。消すときはここから取得したのに対してdestoryを設定すれば良い
	// static -> ボタン押すときの外枠テクスチャ用変数?
	// 敵自体のテクスチャ?ただその場合テクスチャ元とuvを両方保存しないといけない、、
	int m_TextureID = -1; // 敵のテクスチャID
	int m_EnemyID = -1; // 敵のID
	std::pair<int, int> m_TextureTarget{ 0, 0 }; // 対象となるテクスチャの場所(幅、高さ)
	std::pair<int, int> m_TextureCount{ 1, 1 }; // テクスチャの分割数(横、縦) -> これでuvを計算する

	// 描画時にあらかじめいれるテクスチャ等による座標差分を保存
	Vector3 m_PosDiff = { 0.0f, 0.0f, 0.0f }; // 描画時の位置の差分
	Vector3 m_ScaleDiff = { 1.0f, 1.0f, 1.0f }; // 描画時のスケールの差分

	// enemy共通で見るnode以外のステータスを格納する。
	float m_MaxHealth; // 最大体力

};