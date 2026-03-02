#pragma once

#include <list>
#include <memory>
#include "enemy/node_tab/tab_manager.h"
#include "object/ui/button.h"


class FieldEnemy; // 前方宣言
class State;


class EnemyBase
{
public:
	EnemyBase() = default; // デフォルトコンストラクタ
	virtual ~EnemyBase() {}
	EnemyBase(EnemyBase&& Other) noexcept
		: m_TabManager(std::move(Other.m_TabManager))
		, m_ToDnaButton(Other.m_ToDnaButton)
		, m_TextureID(Other.m_TextureID)
		, m_EnemyID(Other.m_EnemyID)
		, m_TextureSplitCount(Other.m_TextureSplitCount)
		, m_UVPos(Other.m_UVPos)
		, m_TextureTarget(Other.m_TextureTarget)
		, m_TextureCount(Other.m_TextureCount)
		, m_PosDiff(Other.m_PosDiff)
		, m_ScaleDiff(Other.m_ScaleDiff)
		, m_IsExitDnaEdit(Other.m_IsExitDnaEdit)
		, m_MaxHealth(Other.m_MaxHealth)
	{
		Other.m_ToDnaButton = nullptr;
		Other.m_TextureID = -1;
		Other.m_EnemyID = -1;
	}
	EnemyBase& operator=(EnemyBase&& Other) noexcept
	{
		if (this != &Other)
		{
			m_TabManager = std::move(Other.m_TabManager);
			m_ToDnaButton = Other.m_ToDnaButton;
			m_TextureID = Other.m_TextureID;
			m_EnemyID = Other.m_EnemyID;
			m_TextureSplitCount = Other.m_TextureSplitCount;
			m_UVPos = Other.m_UVPos;
			m_TextureTarget = Other.m_TextureTarget;
			m_TextureCount = Other.m_TextureCount;
			m_PosDiff = Other.m_PosDiff;
			m_ScaleDiff = Other.m_ScaleDiff;
			m_IsExitDnaEdit = Other.m_IsExitDnaEdit;
			m_MaxHealth = Other.m_MaxHealth;
			Other.m_ToDnaButton = nullptr;
			Other.m_TextureID = -1;
			Other.m_EnemyID = -1;
		}
		return *this;
	}

	virtual EnemyBase* Register(const unsigned int& playerId); // 登録処理
	void Unregister(); // 登録解除処理
	// ノードの内容を下に行動させる関数(攻撃、動き、死亡時)
	void ExecuteAttack(FieldEnemy* enemy_ptr);
	void ExecuteMove(FieldEnemy* enemy_ptr);
	bool ExecuteDeath(FieldEnemy* enemy_ptr);

	// DnaScreenにあるオブジェクトを操作するための関数群
	EnemyBase* Init(const unsigned int& playerId);
	void Uninit();
	void Update();
	void Draw();


	// 敵リストタブ->DNAタブに遷移するボタンを表示するための関数
	void ShowDnaEditButton(const Vector2& pos, const Vector2& size, const int texID, State* ptr);
	void HideDnaEditButton();
	// テクスチャid版がほしい

	// DNAタブ関連の関数
	inline TabManager* GetTabManager() 
	{
		if(m_TabManager)
		{ 
			return m_TabManager.get();
		}
		return nullptr;
	}
	void ShowDnaScreen(); // 呼ばれたらコイツ自身のnode情報を持っているscriptを表示
	void HideDnaScreen();

	inline void SetEnemyID(int id) { m_EnemyID = id; }
	inline int GetEnemyID() const { return m_EnemyID; }
	// 型ごとに一意なIDを生成
	template <typename T>
	static int GetEnemyTypeId()
	{
		static int id = NextEnemyTypeId();
		return id;
	}

	int SetTextureID(const std::wstring filePath, std::pair<int, int> texTarget = {0, 0}, std::pair<int, int> texCount = {1, 1});
	inline const int GetEnemyTextureID() const { return m_TextureID; }

	inline void SetTextureSplitCount(const Vector2& count) { m_TextureSplitCount = count; }
	inline const Vector2& GetTextureSplitCount() const { return m_TextureSplitCount; }
	inline void SetUVPos(const Vector2& uv) { m_UVPos = uv; }
	inline const Vector2& GetUVPos() const { return m_UVPos; }

	// setはとりあえずglobalに。今は使わないかもだけど後々scaleに応じて体力設定とかしたいなら使う。
	inline void SetMaxHealth(const float& maxHealth) { m_MaxHealth = maxHealth; }
	inline const float GetMaxHealth() const { return m_MaxHealth; }
	inline void SetIsExitDnaEdit(const bool isExit) { m_IsExitDnaEdit = isExit; }
	inline const bool GetIsExitDnaEdit() const { return m_IsExitDnaEdit; }

	inline void SetDrawPosDiff(const Vector3& posDiff) { m_PosDiff = posDiff; }
	inline const Vector3& GetDrawPosDiff() const { return m_PosDiff; }
	inline void SetDrawScaleDiff(const Vector3& scaleDiff) {m_ScaleDiff = scaleDiff;}
	inline const Vector3& GetDrawScaleDiff() const { return m_ScaleDiff; }
protected:
	constexpr static Vector2 DEFAULT_TEXTURE_COUNT = Vector2(12.0f, 13.0f); // デフォルトのテクスチャ分割数
private:
	// 敵種類IDのカウンターを管理する静的関数
	static int NextEnemyTypeId()
	{
		static int id = 0;
		return id++;
	}

	std::unique_ptr<TabManager> m_TabManager; // DNAスクリーンのスクリプトオブジェクト(自身が管理している場合はここに保存)

	Button* m_ToDnaButton = nullptr; // 生成したボタンオブジェクトのポインタ。scene側に保持している物のポインタとなる。消すときはここから取得したのに対してdestoryを設定すれば良い
	int m_TextureID = -1; // 敵のテクスチャID
	int m_EnemyID = -1; // 敵のID
	Vector2 m_TextureSplitCount = DEFAULT_TEXTURE_COUNT; // テクスチャの分割数
	Vector2 m_UVPos = Vector2(0.0f, 0.0f); // UVの位置
	std::pair<int, int> m_TextureTarget{ 0, 0 }; // 対象となるテクスチャの場所(幅、高さ)
	std::pair<int, int> m_TextureCount{ 1, 1 }; // テクスチャの分割数(横、縦) -> これでuvを計算する

	// 描画時にあらかじめいれるテクスチャ等による座標差分を保存
	Vector3 m_PosDiff = { 0.0f, 0.0f, 0.0f }; // 描画時の位置の差分
	Vector3 m_ScaleDiff = { 1.0f, 1.0f, 1.0f }; // 描画時のスケールの差分

	bool m_IsExitDnaEdit = false; // DNA編集画面から退出したかどうかのフラグ
	
	// enemy共通で見るnode以外のステータスを格納する。
	float m_MaxHealth = 0; // 最大体力

};