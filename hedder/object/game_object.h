#pragma once

#include "vector3.h"
#include "main.h" // あんまりこれで読み込みたくないんだよな、、
#include "renderer.h"
#include <string>

class GameObject
{
private:
	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };
	bool m_Destory = false; // 削除予約フラグ(今は別の方法で検知している為使っていない)
	int m_TextureID = -1;
	std::string m_Tag; // タグを設定してグループで判定できるように

	// 描画系変数
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;


	// ここに描画系の簡易関数を作成する
	// 後shaderのポインタも持ってていいね
protected:
	void SetVertexBuffer(ID3D11Buffer* VertexBuffer) { m_VertexBuffer = VertexBuffer; }
	ID3D11Buffer* GetVertexBuffer() const { return m_VertexBuffer; }
	ID3D11Buffer** GetVertexBufferPointer() { return &m_VertexBuffer; }
	void SetVertexShader(ID3D11VertexShader* VertexShader) { m_VertexShader = VertexShader; }
	void SetPixelShader(ID3D11PixelShader* PixelShader) { m_PixelShader = PixelShader; }
	void SetVertexLayout(ID3D11InputLayout* VertexLayout) { m_VertexLayout = VertexLayout; }
	void UninitDrawMember();

	void SetCanChangeVertex(); // 頂点データが変更可能にできるプリセット

	// 頂点バッファを描画時に設定する関数
	void SetDefaultVertexBufferOnDraw() const;
	void ChangeTexUV(int texWidthCount, int texHeightCount, int widthTarget, int heightTarget);
	void SetVertexBufferOnDraw() const;
	void SetWorldMatrixOnDraw();
	void SetWorldMatrixOnDrawBillboard(); // ビルボード用のワールドマトリックス設定
	void SetProjectionMatrixOnDraw();
	void SetViewMatrixOnDraw();
	void SetMaterialOnDraw(XMFLOAT4 diff = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4 amb = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), bool texEnable = true);

public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	Vector3& GetPosition() { return m_Position; }
	void SetPosition(Vector3 Position) { m_Position = Position; }
	Vector3& GetRotation() { return m_Rotation; }
	void SetRotation(Vector3 Rotation) { m_Rotation = Rotation; }
	Vector3& GetScale() { return m_Scale; }
	void SetScale(Vector3 Scale) { m_Scale = Scale; }
	int GetTextureID() const { return m_TextureID; }
	void SetTextureID(int TextureID) { m_TextureID = TextureID; }
	std::string GetTag() const { return m_Tag; }
	void SetTag(const std::string& tag) { m_Tag = tag; }

	void SetDestory(bool Destory) { m_Destory = Destory; }
	bool IsDestory() const { return m_Destory; }
	bool Destory()
	{ 
		if (m_Destory)
		{
			Uninit();
			return true; // 削除予約フラグが立っていたらtrueを返す
		}
		return false;
	}


	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetForward() const;
	float GetDistance(Vector3 Position);
};