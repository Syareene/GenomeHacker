#include "main.h"
#include "enemy/node/base.h"
#include "scene/state/dna_edit_state.h"
#include "scene/manager.h"
#include "lib/mouse.h"
#include "object/ui/font.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"

void NodeBase::Init(Transform trans)
{
	SetTransform(trans);
	SetTextureID(TextureManager::LoadTexture(L"asset\\texture\\debug_sprite.png"));

	// ここに説明文格納する感じかな
}

void NodeBase::Uninit()
{

}

void NodeBase::Update()
{
	// ノードの実態としてあるなら判定を行う->dna_editなら実態ありとして判断?
	DnaEditState* dnaState = static_cast<DnaEditState*>(Manager::GetCurrentScene()->GetStatePtr());
	if (!dnaState)
	{
		return;
	}

	// dna_edit stateでかつフォントデータが生成されていないなら生成

	// プレイヤーが自身の範囲内にてクリックしたかどうかを判定
	Vector2 startPos = Vector2(GetPosition().x - (GetScale().x * 0.5f), GetPosition().y - (GetScale().y * 0.5f));
	Vector2 endPos = Vector2(GetPosition().x + (GetScale().x * 0.5f), GetPosition().y + (GetScale().y * 0.5f));

	// マウス座標がノード内にあるかどうか
	if (Mouse::IsMouseInsideArea(startPos, endPos))
	{
		// その状態で左クリックされたかどうか
		if (Mouse::IsLeftButtonTrigger())
		{
			// 現在掴んでいるノードがあるかどうかを確認
			NodeBase* grabbingNode = dnaState->GetGrabbingNode();
			if (grabbingNode)
			{
				// 既に掴んでいるノードがある場合は離す
				dnaState->SetGrabbingNode(nullptr);
				return;
			}
			else
			{
				// 掴んでいるノードがない場合、自身を掴んでいるノードとして設定
				dnaState->SetGrabbingNode(this);
			}
		}
	}

	if(NodeBase* grabbingNode = dnaState->GetGrabbingNode())
	{
		// 掴んでいるノードがある場合、そのノードをマウス位置に移動させる
		if (grabbingNode == this)
		{
			Vector2 mousePos = Mouse::GetPosition();
			Vector3 pos = Vector3(mousePos.x, mousePos.y, 0.0f);
			SetPosition(pos);
		}
	}

}

void NodeBase::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::NoAlphaVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::NoAlphaVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::NoAlphaPixelShader, NULL, 0);

	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();

	// マテリアル設定
	SetMaterialOnDraw();

	Vector3 scale = GetTransform().GetScale();

	// 頂点バッファ設定
	SetDefaultVertexBufferOnDraw();

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ノード自体の描画処理

	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする

	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);



	// ノードソケットの描画処理
}

void NodeBase::MoveNodeToMouse()
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
	DnaEditState* dnaState = static_cast<DnaEditState*>(Manager::GetCurrentScene()->GetStatePtr()); // 前で確認しているので安全に取得できる
	NodeBase* grabbingNode = dnaState->GetGrabbingNode();
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