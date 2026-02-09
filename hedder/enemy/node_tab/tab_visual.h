#pragma once

#include "object/panel.h"
#include "enemy/node/visual_base.h"

class TabBase; // 前方宣言
class EnemyBase;

class TabVisual : public Panel
{
public:
	// 途中で変更はいるからそのタイミングをどうしようかなって感じだ
	void CreateVisual(TabBase* base);
	// 再生成する関数(更新したい対象だけに対して作動)
	void UpdateVisual(TabBase* base);
	void Init(const unsigned int& screen_id, const unsigned int& player_id, TabBase* tab_base);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	inline void SetIsSelected(const bool isSelected) { m_IsSelected = isSelected; } // 現在選択されているタブかどうかを設定
	inline const bool GetIsSelected() const { return m_IsSelected; } // 現在選択されているタブかどうかを取得
	// クリックされたときの処理
	virtual void Clicked()
	{
		// 今のところ特に何もしない
	}

	// index基準でnodeの位置を修正
	void ModifyNodePos(); // enemyとplayer両方修正する関数
	void ApplyGrabNode();
	void ApplyMovedResult();
	// ノード掴んでる時に掴んだノード基準でtab内のnode見て見た目含めindexを修正
	void ModifyEnemyNodeIndexFromPos(Vector2 mousePos, VisualBase* grabPtr);
	void ModifyPlayerNodeIndexFromPos(Vector2 mousePos, VisualBase* grabPtr);

	void SetPlayerId(const unsigned int& playerId) { m_PlayerId = playerId; }
	void SetDnaScreenId(const unsigned int& dnaScreenId) { m_DnaScreenId = dnaScreenId; }

private:
	// 敵エリアは0,180~768,720/プレイヤーエリアは768,0~1280,720
	void ModifyEnemyNodePos(VisualBase* grabPtr = nullptr);
	void ModifyPlayerNodePos(VisualBase* grabPtr = nullptr);
	constexpr static Vector2 ENEMY_NODE_START = { 20.0f, 275.0f }; // ノードと文字の余白
	constexpr static Vector2 PLAYER_NODE_START = { 800.0f, 300.0f }; // ノードの初期配置位置
	constexpr static Vector2 ENEMY_AREA_END = { 768.0f, 720.0f }; // 敵エリアの終了位置
	constexpr static Vector2 PLAYER_AREA_END = { 1280.0f, 720.0f }; // プレイヤーエリアの終了位置
	bool m_IsSelected = false; // 現在選択されているタブかどうか

	unsigned int m_PlayerId = 0; // プレイヤーID
	unsigned int m_DnaScreenId = 0; // dna_screenのID

	std::list<std::unique_ptr<VisualBase>> m_VisualNodes; // 見た目用ノードのリスト

	TabBase* m_Tab = nullptr;
};