#include "main.h"
#include "object/ui/button.h"
#include "lib/input.h"
#include "lib/mouse.h"
#include "manager/texture_manager.h"
#include "manager/shader_manager.h"

void Button::Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const std::wstring& filePath, const std::wstring& frameTexPath)
{
	// ボタンの初期化処理
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
	SetScale(Vector3(scale.x, scale.y, 1.0f));
	SetRotation(Vector3(rot.x, rot.y, 0.0f));
	if (!filePath.empty())
	{
		// テクスチャが指定されている場合、テクスチャIDを設定
		SetTextureID(TextureManager::LoadTexture(filePath));
	}
	else
	{
		SetTextureID(-1); // テクスチャがない場合は-1に設定
	}

	if (!frameTexPath.empty())
	{
		// フレームテクスチャが指定されている場合、フレームテクスチャIDを設定
		m_FrameTexID = TextureManager::LoadTexture(frameTexPath);
	}
	else
	{
		m_FrameTexID = -1; // フレームテクスチャがない場合は-1に設定
	}
	SetNoUpdate(false); // 更新しないが有効な状態にする
	SetActive(true); // アクティブにする
	m_TargetFunc = func; // コールバック関数を設定
}

void Button::Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const int& texID, const std::wstring frameTexPath)
{
	// ボタンの初期化処理
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
	SetScale(Vector3(scale.x, scale.y, 1.0f));
	SetRotation(Vector3(rot.x, rot.y, 0.0f));
	SetTextureID(texID); // テクスチャIDを直接設定
	if (!frameTexPath.empty())
	{
		// フレームテクスチャが指定されている場合、フレームテクスチャIDを設定
		m_FrameTexID = TextureManager::LoadTexture(frameTexPath);
	}
	else
	{
		m_FrameTexID = -1; // フレームテクスチャがない場合は-1に設定
	}
	SetNoUpdate(false); // 更新しないが有効な状態にする
	SetActive(true); // アクティブにする
	m_TargetFunc = func; // コールバック関数を設定
}

void Button::Register(const std::function<void()>& func, const Vector2& pos, const Vector2& scale, const Vector2& rot, const FontData& fontData, const std::string& text, const std::wstring& filePath, const std::wstring& frameTexPath)
{
	// ボタンの初期化処理
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
	SetScale(Vector3(scale.x, scale.y, 1.0f));
	SetRotation(Vector3(rot.x, rot.y, 0.0f));
	if (!filePath.empty())
	{
		// テクスチャが指定されている場合、テクスチャIDを設定
		SetTextureID(TextureManager::LoadTexture(filePath));
	}
	else
	{
		SetTextureID(-1); // テクスチャがない場合は-1に設定
	}

	if (!frameTexPath.empty())
	{
		// フレームテクスチャが指定されている場合、フレームテクスチャIDを設定
		m_FrameTexID = TextureManager::LoadTexture(frameTexPath);
	}
	else
	{
		m_FrameTexID = -1; // フレームテクスチャがない場合は-1に設定
	}
	// テキストオブジェクトを作成
	// ->これ操体市で生成できるようにしてないから位置がどうかなーといったところ
	m_Text = std::make_unique<Font>();
	m_Text->Init();
	m_Text->Register(Vector2(pos.x, pos.y), fontData, text);
	SetNoUpdate(false); // 更新しないが有効な状態にする
	SetActive(true); // アクティブにする
	m_TargetFunc = func; // コールバック関数を設定
}

void Button::Init(Transform trans)
{
	SetTransform(trans);
}

void Button::Uninit()
{
	m_TargetFunc = nullptr; // コールバック関数をクリア
	m_Text.reset(); // テキストオブジェクトを解放
}

void Button::Update()
{
	if (!IsActive())
	{
		return;
	}

	// ボタンの更新処理
	// クリック検知などを行う

	// 自身の描画範囲内にマウスカーソルがあるかどうかをチェック
	if (Mouse::IsMouseInsideArea(Vector2(GetPosition().x - (GetScale().x * 0.5f), GetPosition().y - (GetScale().y * 0.5f)),
		Vector2(GetPosition().x + (GetScale().x * 0.5f), GetPosition().y + (GetScale().y * 0.5f))) && 
		Mouse::IsLeftButtonTrigger())
	{
		OnClick(); // マウスがボタンの範囲内でクリックされた場合、OnClickを呼び出す
	}

	// テキストがあるならテキストに対して更新
	if (m_Text)
	{
		m_Text->Update();
	}
}

void Button::Draw()
{ 
	if (!IsActive())
	{
		return;
	}

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(ShaderManager::UnlitVertexLayout);
	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(ShaderManager::UnlitVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(ShaderManager::UnlitPixelShader, NULL, 0);
	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	SetDefaultVertexBufferOnDraw(); //->こっちだとuv変えるときに反映できないので下記に
	//SetVertexBufferOnDraw(); // ボタンの頂点バッファを設定(こっちにすると描画されなくなるよーん、なのでデフォルトで頂点いれるように設定直さないといけない)
	// プロジェクションマトリックス設定
	//SetProjectionMatrixOnDraw();
	// ビューマトリックス設定
	SetViewMatrixOnDraw();
	// 移動、回転マトリックス設定
	SetWorldMatrixOnDraw();
	// マテリアル設定
	SetMaterialOnDraw();

	// フレームテクスチャが設定されている場合、フレームテクスチャを設定
	if (m_FrameTexID != -1)
	{
		// テクスチャ設定
		// 一時変数に入れないと参照取得できないのでこうする
		ID3D11ShaderResourceView* texture = TextureManager::GetTexture(m_FrameTexID);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// 描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	// テクスチャがあるときだけ描画
	if(GetTextureID() != -1)
	{
		// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
		ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// 描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}


	// フォントのポインタがあるならフォントを描画
	if (m_Text)
	{
		m_Text->Draw();
	}
}