#pragma once

#include "lib/vector3.h"
#include "main.h" // あんまりこれで読み込みたくないんだよな、、
#include "renderer.h"
//#include "manager.h"
// ->多分ここのせいでエラー出てる
#include <string>
#include <list>

class GameObject
{
private:
	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };
	bool m_IsActive = true; // アクティブフラグ(ここデフォでtrueにするかは検討)
	bool m_Destory = false; // 削除予約フラグ(今は別の方法で検知している為使っていない)
	int m_TextureID = -1;
	std::list<std::string> m_Tag; // タグを設定してグループで判定できるように->listにしても良い
	float m_ObjSpeedMlt = 1.0f; // オブジェクトの速度(ゲーム内での移動速度などに使用)

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

	
	void AddPosition(Vector3 Position, bool calcWorldSpeed = true);
	Vector3& GetPosition() { return m_Position; }
	void SetPosition(Vector3 Position) { m_Position = Position; }
	Vector3& GetVelocity() { return m_Velocity; }
	void SetVelocity(Vector3 Velocity) { m_Velocity = Velocity; }
	Vector3& GetRotation() { return m_Rotation; }
	void SetRotation(Vector3 Rotation) { m_Rotation = Rotation; }
	Vector3& GetScale() { return m_Scale; }
	void SetScale(Vector3 Scale) { m_Scale = Scale; }
	int GetTextureID() const { return m_TextureID; }
	void SetTextureID(int TextureID) { m_TextureID = TextureID; }
	std::list<std::string>& GetTagList() { return m_Tag; }
	bool IsTagAvailable(const std::string& tagName) const
	{
		for (const auto& tag : m_Tag)
		{
			if (tag == tagName)
			{
				return true; // タグが見つかったらtrueを返す
			}
		}
		return false; // 見つからなかった場合
	}
	void AddTag(const std::string& tag) { m_Tag.push_back(tag); }
	void SetObjectSpeedMlt(float speedMlt) { m_ObjSpeedMlt = speedMlt; }
	float GetObjectSpeedMlt() const { return m_ObjSpeedMlt; }
	bool IsActive() const { return m_IsActive; }
	void SetActive(bool IsActive) { m_IsActive = IsActive; }
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