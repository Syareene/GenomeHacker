#include "main.h"
#include "manager/texture_manager.h"
#include "enemy/node/visual_base.h"
#include "enemy/node/base.h"
#include "scene/manager.h"
#include "enemy/dna_screen_script.h"
#include "lib/mouse.h"


// この辺StateでGrabbed管理してるが移行に伴ってvisual_tab側においても良いかも?



void VisualBase::Init(const unsigned int& screen_id, TabVisual* parent_tab, NodeBase* node)
{
	// 登録->Playerがこれを追加したい時にこれ参照できん
	m_ParentTab = parent_tab;

	// screen_id保存
	m_ScreenID = screen_id;

	// 名前部分生成
	SetNameFont(node->GetNameData());
	// 説明文部分生成
	for(auto& desc : node->GetDescriptionData())
	{
		AddDescFont(desc);
	}

	// ここどこから座標持ってくるのかはっきりしておくこと(ぶっちゃけ後で整列するから今いらないのかもしれないけど)
	SetTransform(Transform());
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\debug_sprite.png"));


	UpdateVisual(node);
}

void VisualBase::UpdateVisual(NodeBase* node_ptr)
{
	//m_Transform = node_ptr->GetTransform();
	m_IsUpdated = true;


	Vector2 scale = m_Font.GetWidthHeight();
	Vector3 start_pos = m_Font.GetPosition();
	SetScale(Vector3(scale.x + NODE_MARGIN.x, scale.y + NODE_MARGIN.y, 0.0f));
	//SetScale(Vector3(400.0f, scale.y + NODE_MARGIN.y, 0.0f)); // xは固定で400->フォント真ん中揃えなので位置変えないとダメ
	SetPosition(Vector3(scale.x * 0.5f + start_pos.x, scale.y * 0.5f + start_pos.y, 0.0f));



	// フォントデータ再生成(これ少なくともisUpdatedの下に置かないといけなくて、更新関数は他からも呼ばれる可能性があって)
	// その辺どうするかって感じ、でも再読み込み以外でこれが呼ばれることない気はするけどね(途中で文章変わるみたいな)
	// ただ中身のnode要素に対しての更新っていう意味だとあるかな
}

void VisualBase::Uninit()
{

}

void VisualBase::Update()
{
	// プレイヤーが自身の範囲内にてクリックしたかどうかを判定
	Vector2 startPos = Vector2(GetPosition().x - (GetScale().x * 0.5f), GetPosition().y - (GetScale().y * 0.5f));
	Vector2 endPos = Vector2(GetPosition().x + (GetScale().x * 0.5f), GetPosition().y + (GetScale().y * 0.5f));

	// マウス座標がノード内にあるかどうか
	if (Mouse::IsMouseInsideArea(startPos, endPos))
	{
		// カウントインクリメント
		m_HoverTimer++;

		// その状態で左クリックされたかどうか
		if (Mouse::IsLeftButtonTrigger())
		{
			m_HoverTimer = 0; // ホバータイマーリセット
			// 現在掴んでいるノードがあるかどうかを確認
			VisualBase* grabbingNode = Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();

			Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>();

			if (grabbingNode)
			{
				// 反映処理
				Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->ReleaseGrabbingNode();

				// 既に掴んでいるノードがある場合は離す
				//dnaState->SetGrabbingNode(nullptr);
				//return;
			}
			else
			{
				// 掴んでいるノードがない場合、自身を掴んでいるノードとして設定
				Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->SetGrabbingNode(this);
			}
		}
	}
	else
	{
		// 非表示
		m_HoverTimer = 0;
	}

	if (VisualBase* grabbingNode = Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->GetGrabbingNode())
	{
		m_HoverTimer = 0; // ホバータイマーリセット
		// 掴んでいるノードがある場合、そのノードをマウス位置に移動させる
		if (grabbingNode == this)
		{
			Vector2 mouseDiffPos = Mouse::GetDiffPosition();
			Vector3 pos = Vector3(mouseDiffPos.x + GetPosition().x, mouseDiffPos.y + GetPosition().y, 0.0f);
			SetPosition(pos);
			// 中身のフォントの位置も動かす
			m_Font.SetPosition(Vector3(pos.x - (GetScale().x * 0.5f) + (NODE_MARGIN.x * 0.5f), pos.y - (GetScale().y * 0.5f) + (NODE_MARGIN.x * 0.5f), 0.0f));
		}
	}

	// フォント参照してサイズ更新
	// 今あるノード実装しきったらここはいった時にassertでエラー出す	

	// ホバータイマーが一定値以上なら説明文を表示
	if (m_HoverTimer >= SHOW_DESC_TIME)
	{
		// ノードのちょい上あたりに表示
		m_DescriptionFonts.back().SetPosition(Vector3(GetPosition().x - (GetScale().x * 0.5f) + NODE_MARGIN.x, GetPosition().y - (GetScale().y * 0.5f) - NODE_MARGIN.y - (m_DescriptionFonts.back().GetWidthHeight().y), 0.0f));
	}
}

void VisualBase::Draw()
{
	// 描画
	Renderer::Draw2D(GetTextureID(), GetPosition(), GetScale());

	//Renderer::GetD2DRenderTarget()->Draw

	// フォント描画
	m_Font.Draw();

	// 説明文描画

	// ホバーしてるなら説明文出す
	if (IsShowDesc())
	{
		for (auto& desc : m_DescriptionFonts)
		{
			// 2d描画
			Renderer::Draw2D(GetTextureID(),
				Vector2(desc.GetPosition().x + (desc.GetWidthHeight().x * 0.5f), desc.GetPosition().y + (desc.GetWidthHeight().y * 0.5f)),
				Vector2(desc.GetWidthHeight().x, desc.GetWidthHeight().y));

			// フォント描画
			desc.Draw();
		}
	}

	// ノードソケットの描画処理
}

const FontData& VisualBase::GetFontDataFromTextType(const NodeBase::TextType& type) const
{
	// text_typeを元にFontDataを返す
	// 仮実装
	static FontData normalFontData;
	normalFontData.fontSize = 24;
	normalFontData.fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	normalFontData.Color = D2D1::ColorF(D2D1::ColorF::Black);
	normalFontData.font = DirectWriteCustomFont::GetFontName(0);
	normalFontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	normalFontData.shadowOffset = D2D1::Point2F(2.0f, -2.0f);
	normalFontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	normalFontData.outlineWidth = 1.0f;
	static FontData rareFontData;
	rareFontData.fontSize = 28;
	rareFontData.fontWeight = DWRITE_FONT_WEIGHT_BOLD;
	rareFontData.Color = D2D1::ColorF(D2D1::ColorF::Gold);
	rareFontData.font = DirectWriteCustomFont::GetFontName(1);
	rareFontData.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	rareFontData.shadowOffset = D2D1::Point2F(3.0f, -3.0f);
	rareFontData.outlineColor = D2D1::ColorF(D2D1::ColorF::White);
	rareFontData.outlineWidth = 2.0f;
	switch (type)
	{
	case NodeBase::TextType::Normal:
		return normalFontData;
	case NodeBase::TextType::Rare:
		return rareFontData;
	default:
		return normalFontData;
	}
}

void VisualBase::FixFontPositions(Vector2 diff)
{
	// フォントの位置をdiff分だけ修正

	Vector3 pos = m_Font.GetPosition();
	m_Font.SetPosition(Vector3(pos.x + diff.x, pos.y + diff.y, pos.z));


	//for(auto& fontPtr : m_Fonts)
	//{
	//	Vector3 pos = fontPtr->GetPosition();
	//	fontPtr->SetPosition(Vector3(pos.x + diff.x, pos.y + diff.y, pos.z));
	//}
}

void VisualBase::MoveNodeToMouse()
{
	// ノードの座標に対してクリックされたらマウスを掴み状態に、
	// 掴み状態の場合はこの処理が実行、
	// 掴んでる状態で再度クリックで外す。
	// 親ノードの場合、子ノードの判定を取らないように子ノードから先に判定をする形となる。

	// また、この形にしかくっつかないとかもあるので該当座標のノードがちゃんとガッチャンコされるかどうかとかもチェックしないといけない
	// なのでくっつく場所に対しての空のオブジェクト(判定や位置を持っておく)があるとよいのかもしれない

	// これ変数保存部分か取得部分をtemplateのTで取れるようにすればくっつけられる型が判断できるのと
	// その中(子ノード)に対してgetscaleやposで位置取れるようになるからそれで判定できるね


	// 現在dna_edit state上で掴んでいるノードがあるかどうかを確認
	VisualBase* grabbingNode = Manager::GetCurrentScene()->GetStatePtr()->GetGameObject<DnaScreenScript>()->GetGrabbingNode();
	if (grabbingNode)
	{
		// 自身でないノードが掴まれている場合は処理しない
		if (grabbingNode != this)
		{
			return;
		}
		else
		{
			// 掴んでいるノードが自分自身の場合、マウス位置に移動させる
			Vector2 mousePos = Mouse::GetPosition();
			Vector3 pos = Vector3(mousePos.x, mousePos.y, 0.0f);
			SetPosition(pos);
		}
	}
}