#include "main.h"
#include "enemy/node/base.h"
#include "enemy/base_data/enemy_base.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"
#include "lib/mouse.h"
#include "object/ui/font.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"

#include "imgui.h"

unsigned int NodeBase::m_UniqueIDCounter = 0;

void NodeBase::Init(Transform trans)
{

	// ここに説明文格納する感じかな
}

void NodeBase::ImWindowSettings()
{
	// ウィンドウ位置取得
	RECT screen_rect = GetRect();
	// 起点ポイント
	POINT pt = { screen_rect.right, screen_rect.top };
	// ウィンドウ内座標ではなくスクリーン座標に変換
	ClientToScreen(GetWindow(), &pt);
	// ウィンドウの諸々を設定
	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(pt.x), static_cast<float>(pt.y)), ImGuiCond_Appearing); // これScreenの実posで出す感じ->現在のウィンドウ位置を見て相対的に置く感じかな
	ImGui::SetNextWindowSize(ImVec2(IMGUI_WINDOW_SIZE.x, IMGUI_WINDOW_SIZE.y), ImGuiCond_Appearing);
}

void NodeBase::ShowTabInfo()
{
	ImGui::SeparatorText("UseableTab");
	ImGui::BeginDisabled(true); // 無効化してさわれなくする
	// 各種タブが使えるかどうかを表示
	bool canUseAttackTab = (std::find(m_InputTypesTop.begin(), m_InputTypesTop.end(), InputType::Attack) != m_InputTypesTop.end());
	ImGui::Checkbox("CanUseAttackTab", &canUseAttackTab);
	bool canUseMoveTab = (std::find(m_InputTypesTop.begin(), m_InputTypesTop.end(), InputType::Move) != m_InputTypesTop.end());
	ImGui::Checkbox("CanUseMoveTab", &canUseMoveTab);
	bool canUseDeathTab = (std::find(m_InputTypesTop.begin(), m_InputTypesTop.end(), InputType::Death) != m_InputTypesTop.end());
	ImGui::Checkbox("CanUseDeathTab", &canUseDeathTab);
	ImGui::EndDisabled(); // 無効化終了
}

bool NodeBase::NodeEffect(FieldEnemy* enemy_ptr)
{
	return false;
}

const bool NodeBase::CanAttach(NodeBase* upper_node, NodeBase* lower_node) const
{
	bool canTop = false;
	bool canBottom = false;
	bool isCheckUpper = false;
	bool isCheckLower = false;

	// ノードが存在しない場合はくっつけられないので判定しない
	if (upper_node != nullptr)
	{
		isCheckUpper = true;
	}

	if( lower_node != nullptr)
	{
		isCheckLower = true;
	}

	// 自身の上部にくっつけられるか
	if (isCheckUpper)
	{
		for (const auto& t : m_InputTypesTop)
		{
			// upper_nodeの下部にくっつけられる形と一致するか
			for (const auto& u : upper_node->GetInputTypesBottom())
			{
				if (t == u)
				{
					canTop = true;
					break;
				}
			}
		}
	}
	// 自身の下部にくっつけられるか
	if (isCheckLower)
	{
		for( const auto& t : m_InputTypesBottom)
		{
			// lower_nodeの上部にくっつけられる形と一致するか
			for (const auto& l : lower_node->GetInputTypesTop())
			{
				if (t == l)
				{
					canBottom = true;
					break;
				}
			}
		}
	}

	// 条件に基づいてreturn
	
	// かたっぽだけチェックする場合
	// 下
	if (!isCheckUpper && isCheckLower)
	{
		return canBottom;
	}
	// 上
	else if (isCheckUpper && !isCheckLower)
	{
		return canTop;
	}
	// 両方
	else if (isCheckUpper && isCheckLower)
	{
		return (canTop && canBottom);
	}

	// 両方ともnullptr->チェックしなくていい場合はtrueを返す(初期状態とかこうなるしね)
	return true;
}

void NodeBase::UpdateDescriptionData()
{
	// テキスト生成等
	// これtemplate化してargs受け取ればargある場合にstd::formatsで変数埋込できるね
}