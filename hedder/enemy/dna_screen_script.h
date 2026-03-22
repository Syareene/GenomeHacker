#pragma once

#include "object/panel.h"
#include "enemy/node_tab/tab_visual.h"

class EnemyBase;

// 見た目部分を管理するクラス
class DnaScreenScript : public Panel
{
public:

	// このクラスがenemy_baseからstateにたされてるので要らなくなったらstateからdestoryすべきかな

	static constexpr bool ENABLE_INSTANCING = false;

	DnaScreenScript() = default;
	~DnaScreenScript() override = default;
	
	// ムーブコンストラクタ
	DnaScreenScript(DnaScreenScript&& Other) noexcept
		: Panel(std::move(Other))
		, m_AttackVisual(std::move(Other.m_AttackVisual))
		, m_MoveVisual(std::move(Other.m_MoveVisual))
		, m_DeathVisual(std::move(Other.m_DeathVisual))
		, m_PlayerId(std::exchange(Other.m_PlayerId, 0))
		, m_GrabbingNode(std::exchange(Other.m_GrabbingNode, nullptr))
		, m_InfoNodePtr(std::exchange(Other.m_InfoNodePtr, nullptr))
		, m_IsReleaseGrabNode(std::exchange(Other.m_IsReleaseGrabNode, false))
		, m_EnemyBase(std::exchange(Other.m_EnemyBase, nullptr))
	{
		// すべて初期化リストで完結
	}

	// ムーブ代入演算子
	DnaScreenScript& operator=(DnaScreenScript&& Other) noexcept
	{
		if (this != &Other)
		{
			Panel::operator=(std::move(Other));
			m_AttackVisual = std::move(Other.m_AttackVisual);
			m_MoveVisual = std::move(Other.m_MoveVisual);
			m_DeathVisual = std::move(Other.m_DeathVisual);
			m_PlayerId = Other.m_PlayerId;
			m_GrabbingNode = Other.m_GrabbingNode;
			m_InfoNodePtr = Other.m_InfoNodePtr;
			m_IsReleaseGrabNode = Other.m_IsReleaseGrabNode;
			m_EnemyBase = Other.m_EnemyBase;
			
			Other.m_GrabbingNode = nullptr;
			Other.m_InfoNodePtr = nullptr;
			Other.m_EnemyBase = nullptr;
		}
		return *this;
	}

	void Init(EnemyBase* base_enemy, const unsigned int& player_id);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	// DNAスクリーンの特定の処理を追加
	void ShowDnaInfo();
	void HideDnaInfo();

	// タブ移動する時に掴んでるノードは解放してね!
	inline VisualBase* GetGrabbingNode() const { return m_GrabbingNode; }
	inline void ReleaseGrabbingNode() { m_IsReleaseGrabNode = true; }
	inline void SetGrabbingNode(VisualBase* nodePtr) { m_GrabbingNode = nodePtr; }

	inline VisualBase* GetInfoNode() const { return m_InfoNodePtr; }
	inline void ReleaseInfoNode() { m_InfoNodePtr = nullptr; }
	inline void SetInfoNode(VisualBase* nodePtr) { m_InfoNodePtr = nodePtr; }

	TabVisual* GetActiveTab();

	// どのタブが選択されているかを取得する関数
	NodeBase::InputType GetActiveTabType()
	{
		if (m_AttackVisual.GetIsSelected())
		{
			return NodeBase::InputType::Attack;
		}
		else if (m_MoveVisual.GetIsSelected())
		{
			return NodeBase::InputType::Move;
		}
		else if (m_DeathVisual.GetIsSelected())
		{
			return NodeBase::InputType::Death;
		}
		return NodeBase::InputType::None; // どのタブも選択されていない場合
	}

	inline TabVisual* GetAttackTabVisual() { return &m_AttackVisual; }
	inline TabVisual* GetMoveTabVisual() { return &m_MoveVisual; }
	inline TabVisual* GetDeathTabVisual() { return &m_DeathVisual; }

private:
	TabVisual m_AttackVisual;
	TabVisual m_MoveVisual;
	TabVisual m_DeathVisual;

	void GeneratePlayerVisualNodes();

	FontData GetTabFontData(NodeBase::InputType type)
	{
		// タブの種類に応じてフォントデータを返す

		// 共通のデータ
		FontData fontData;
		fontData.fontSize = 100;
		fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
		fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
		fontData.font = DirectWriteCustomFont::GetFontName(0);
		fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
		fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
		fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
		fontData.outlineWidth = 12.0f;

		// タブに応じて色を変更
		switch (type)
		{
			case NodeBase::InputType::Attack:
				fontData.Color = D2D1::ColorF(D2D1::ColorF::OrangeRed);
				break;
			case NodeBase::InputType::Move:
				fontData.Color = D2D1::ColorF(D2D1::ColorF::ColorF::ForestGreen);
				break;
			case NodeBase::InputType::Death:
				fontData.Color = D2D1::ColorF(D2D1::ColorF::DarkSlateBlue);
				break;
			default:
				fontData.Color = D2D1::ColorF(D2D1::ColorF::White);
				break;
		}
		return fontData;
	}

	void SelectedAttackTab();
	void SelectedMoveTab();
	void SelectedDeathTab();

	// プレイヤーのid(tab_visual用に保存し、playerのノードを見れるようにする)
	unsigned int m_PlayerId = 0;

	VisualBase* m_GrabbingNode = nullptr; // 現在掴んでいるノードのポインタ
	VisualBase* m_InfoNodePtr = nullptr; // デバッグ用に変数をいじるウィンドウを表示しているノードのポインタ
	bool m_IsReleaseGrabNode = false; // ノードを離したかどうかのフラグ
	EnemyBase* m_EnemyBase = nullptr; // 参照しているデータ
	static constexpr Vector2 NODE_TAB_TEXT_POS = { 1000.0f, 35.0f }; // ノードタブのテキスト位置
	static constexpr Vector2 PLAYER_NODE_LIST_POS = { 1024.0f, 450.0f }; // プレイヤーノード欄の位置
	static constexpr Vector2 PLAYER_NODE_LIST_SCALE = { 512.0f, 540.0f }; // プレイヤーノード欄のスケール
	static constexpr Vector2 ENEMY_VISUAL_POS = { 680.0f, 640.0f }; // 敵の見た目の位置
	static constexpr Vector2 ENEMY_VISUAL_SCALE = { 125.0f, 125.0f }; // 敵の見た目のスケール
	static constexpr Vector2 ENEMY_NODE_LIST_POS = { 384.0f, 450.0f }; // 敵ノード欄の位置
	static constexpr Vector2 ENEMY_NODE_LIST_SCALE = { 768.0f, 540.0f }; // 敵ノード欄のスケール
	static constexpr Vector2 TAB_BUTTON_SIZE = {100.0f, 50.0f};
};