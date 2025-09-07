#include "object/ui/button.h"
#include "input.h"
#include "lib/mouse.h"
#include "texture_manager.h"
#include "shader_manager.h"

void Button::Register(const std::function<void()>& func, Vector2 pos, Vector2 scale, Vector2 rot, const std::wstring filePath, const std::wstring frameTexPath)
{
	// ボタンの初期化処理
	SetPosition(Vector3(pos.x, pos.y, 0.0f));
	SetScale(Vector3(scale.x, scale.y, 1.0f));
	SetRotation(Vector3(rot.x, rot.y, 0.0f));
	SetTextureID(TextureManager::LoadTexture(filePath));
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

void Button::Register(const std::function<void()>& func, Vector2 pos, Vector2 scale, Vector2 rot, int texID, const std::wstring frameTexPath)
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

void Button::Init()
{
	
}

void Button::Uninit()
{
	m_TargetFunc = nullptr; // コールバック関数をクリア
}

void Button::Update()
{
	// ボタンの更新処理
	// クリック検知などを行う

	// 自身の描画範囲内にマウスカーソルがあるかどうかをチェック
	if (Mouse::IsMouseInsideArea(Vector2(GetPosition().x - (GetScale().x * 0.5f), GetPosition().y - (GetScale().y * 0.5f)),
		Vector2(GetPosition().x + (GetScale().x * 0.5f), GetPosition().y + (GetScale().y * 0.5f))) && 
		Mouse::IsLeftButtonTrigger())
	{
		OnClick(); // マウスがボタンの範囲内でクリックされた場合、OnClickを呼び出す
	}
}

void Button::Draw()
{ 
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


	// テクスチャ設定
	// 一時変数に入れないと参照取得できないのでこうする
	ID3D11ShaderResourceView* texture = TextureManager::GetTexture(GetTextureID());
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}