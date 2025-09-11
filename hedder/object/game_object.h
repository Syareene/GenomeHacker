#pragma once

#include "lib/transform.h"
#include "main.h" // あんまりこれで読み込みたくないんだよな、、
#include "renderer.h"
//#include "manager.h"
// ->多分ここのせいでエラー出てる
#include <string>
#include <list>

#include <Windows.h>

class GameObject
{
private:
	Transform m_Transform = Transform();
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	bool m_IsActive = true; // アクティブフラグ(ここデフォでtrueにするかは検討)
	bool m_Destroy = false; // 削除予約フラグ(今は別の方法で検知している為使っていない)
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
	inline void SetVertexBuffer(ID3D11Buffer* VertexBuffer) { m_VertexBuffer = VertexBuffer; }
	inline ID3D11Buffer* GetVertexBuffer() const { return m_VertexBuffer; }
	inline ID3D11Buffer** GetVertexBufferPointer() { return &m_VertexBuffer; }
	inline void SetVertexShader(ID3D11VertexShader* VertexShader) { m_VertexShader = VertexShader; }
	inline void SetPixelShader(ID3D11PixelShader* PixelShader) { m_PixelShader = PixelShader; }
	inline void SetVertexLayout(ID3D11InputLayout* VertexLayout) { m_VertexLayout = VertexLayout; }
	void UninitDrawMember();
	void SetCanChangeVertex(); // 頂点データが変更可能にできるプリセット

	// 頂点バッファを描画時に設定する関数
	void SetDefaultVertexBufferOnDraw() const;
	void SetDefaultVertexBufferBillboardOnDraw() const;
	void SetVertexBufferOnDraw() const;
	void SetWorldMatrixOnDraw();
	void SetWorldMatrixOnDrawBillboard(); // ビルボード用のワールドマトリックス設定
	void SetProjectionMatrixOnDraw();
	void SetViewMatrixOnDraw();
	void SetMaterialOnDraw(const XMFLOAT4& diff = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), const XMFLOAT4& amb = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), const bool& texEnable = true);

public:
	virtual ~GameObject() {}
	virtual void Init(Transform trans = Transform()) 
	{
		SetTransform(trans);
	};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	void ChangeTexUV(int texWidthCount, int texHeightCount, int widthTarget, int heightTarget);

	// get/set系関数(軽いものはinlineをつけ、get/setの適切な部分にconstをつけること!)

	inline void AddPosition(const Vector3& Position, const bool& calcWorldSpeed = true);
	inline const Vector3& GetPosition() const { return m_Transform.GetPosition(); }
	inline void SetPosition(const Vector3& Position) { m_Transform.SetPosition(Position); }
	inline Vector3& GetVelocity() { return m_Velocity; }
	inline void SetVelocity(const Vector3& Velocity) { m_Velocity = Velocity; }
	inline const Vector3& GetRotation() const { return m_Transform.GetRotation(); }
	inline void SetRotation(const Vector3& Rotation) { m_Transform.SetRotation(Rotation); }
	inline const Vector3& GetScale() const { return m_Transform.GetScale(); }
	inline void SetScale(const Vector3& Scale) { m_Transform.SetScale(Scale); }
	inline const Vector3& GetRadian() const { return m_Transform.GetRadian(); }
	inline const Transform& GetTransform() const { return m_Transform; }
	inline void SetTransform(const Transform& Transform) { m_Transform = Transform; }
	inline int GetTextureID() const { return m_TextureID; }
	inline void SetTextureID(const int& TextureID) { m_TextureID = TextureID; }
	inline std::list<std::string>& GetTagList() { return m_Tag; }
	inline bool IsTagAvailable(const std::string& tagName) const
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
	inline void AddTag(const std::string& tag) { m_Tag.push_back(tag); }
	inline void SetObjectSpeedMlt(const float& speedMlt) { m_ObjSpeedMlt = speedMlt; }
	inline const float GetObjectSpeedMlt() const { return m_ObjSpeedMlt; }
	inline const bool IsActive() const { return m_IsActive; }
	inline void SetActive(const bool& IsActive) { m_IsActive = IsActive; }
	inline void SetDestory(const bool& Destroy) { m_Destroy = Destroy; }
	inline const bool IsDestory() const { return m_Destroy; }
	inline bool Destroy()
	{ 
		if (m_Destroy)
		{
			Uninit();
			return true; // 削除予約フラグが立っていたらtrueを返す
		}
		return false;
	}

	const Vector3 GetRight() const;
	const Vector3 GetUp() const;
	const Vector3 GetForward() const;
	float GetDistance(const Vector3& Position) const;
};