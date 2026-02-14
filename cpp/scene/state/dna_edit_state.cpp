#include "main.h"
#include "scene/state/dna_edit_state.h"
#include "scene/state/dna_table_state.h"
#include "scene/manager.h"
#include "scene/base_scene.h"
#include "object/ui/button.h"
#include "manager/texture_manager.h"
#include "enemy/base_data/enemy_base.h"
#include "enemy/node_tab/togame_button.h"

#include "enemy/node/base.h"

void DnaEditState::Init()
{
	if (IsInitialized())
	{
		return;
	}
	SetIsInitialized(true);

	State::Init();
	// sceneの描画タグリセット
	Manager::GetCurrentScene()->GetDrawTargetTags().clear();
	// sceneの描画タグセット
	Manager::GetCurrentScene()->AddDrawTargetTag("dna_edit");
	// stateの描画タグリセット
	GetDrawTargetTags().clear();
	// stateの描画タグセット(このstateはない)
	//AddDrawTargetTag();

	// フォントデータ
	FontData fontData;
	fontData.fontSize = 50;
	fontData.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	fontData.textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	fontData.Color = D2D1::ColorF(D2D1::ColorF::Gold);
	fontData.font = DirectWriteCustomFont::GetFontName(0);
	fontData.shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
	fontData.shadowOffset = D2D1::Point2F(5.0f, -5.0f);
	fontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	fontData.outlineWidth = 6.0f;
	// テキストボタンの登録
	AddGameObject<Button>(0)->Register([]()
	{
		auto currentState = Manager::GetCurrentScene()->GetCurrentState();
		if (auto dnaEditState = dynamic_cast<DnaEditState*>(currentState))
		{
			dnaEditState->Clicked();
		}
	}, ToGameButton::BUTTON_POS, ToGameButton::BUTTON_SIZE, Vector2(0.0f, 0.0f),
		fontData, "戻る", L"asset\\texture\\alpha_texture.png");

	// 仮登録を本登録へ
	FlushPendingObjects();
}

void DnaEditState::Uninit()
{
	// override後、個別で追加したいものの追加
	State::Uninit();
}

void DnaEditState::Update()
{
	State::Update();

	Manager::GetCurrentScene()->UpdateObjectByTag("dna_edit");
	UpdateStateObject();
}

void DnaEditState::Draw()
{
	State::Draw();
	// updateと同じ
	Manager::GetCurrentScene()->DrawObjectByTag("dna_edit");

	// state内オブジェクトの描画
	DrawStateObject();

	State::UpdateFinal();
}

void DnaEditState::Clicked()
{
	// state変更
	Manager::GetCurrentScene()->SetState<DnaTableState>();
}


// tab_baseのModifyNodeIndexFromPosみたいなしょうに関数作ってその中でこの関数実行するようにする
// んでそしたらちゃんと動く
void DnaEditState::ModifyNodePos()
{

}