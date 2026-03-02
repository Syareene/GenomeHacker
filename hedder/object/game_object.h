#pragma once

#include "main.h"
#include "lib/transform.h"
#include "lib/renderer.h"
#include "object/gpu_data.h"
//#include "scene/manager.h"
// ->多分ここのせいでエラー出てる
#include <string>
#include <list>
#include <deque>
#include <Windows.h>

class IGameObjectManager; // 前方宣言

class GameObject
{
private:
	static unsigned int m_NextObjectID; // 次に割り当てるオブジェクトID
	Transform m_Transform = Transform();
	Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f };
	bool m_IsAliveData = true; // 実体が存在するかどうか(ObjectManagerで管理している場合は削除はせず配列上に残しておく)
	bool m_IsActive = true; // アクティブフラグ(ここデフォでtrueにするかは検討)
	bool m_Destroy = false; // 削除予約フラグ(今は別の方法で検知している為使っていない)
	int m_TextureID = -1;
	int m_Layer = 0; // 描画レイヤー(数値が大きいほど手前側)
	unsigned int m_ObjectID = 0; // オブジェクトID(管理用)
	std::deque<std::string> m_Tag; // タグを設定してグループで判定できるように->listにしても良い
	float m_ObjSpeedMlt = 1.0f; // オブジェクトの速度(ゲーム内での移動速度などに使用)

	// 描画系変数
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	// ここに描画系の簡易関数を作成する
protected:
	inline ID3D11Buffer** GetVertexBufferPointer() { return &m_VertexBuffer; }
	inline void SetVertexShader(ID3D11VertexShader* VertexShader) { m_VertexShader = VertexShader; }
	inline ID3D11VertexShader* GetVertexShader() const { return m_VertexShader; }
	inline ID3D11VertexShader** GetVertexShaderPointer() { return &m_VertexShader; }
	inline void SetPixelShader(ID3D11PixelShader* PixelShader) { m_PixelShader = PixelShader; }
	inline ID3D11PixelShader* GetPixelShader() const { return m_PixelShader; }
	inline ID3D11PixelShader** GetPixelShaderPointer() { return &m_PixelShader; }
	inline void SetVertexLayout(ID3D11InputLayout* VertexLayout) { m_VertexLayout = VertexLayout; }
	inline ID3D11InputLayout* GetVertexLayout() const { return m_VertexLayout; }
	inline ID3D11InputLayout** GetVertexLayoutPointer() { return &m_VertexLayout; }
	void UninitDrawMember();

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
	static constexpr size_t MAX_OBJECTS = 64; // オブジェクトvector最大数。継承先クラスで変更可能。
	static constexpr bool ENABLE_INSTANCING = true;
	static constexpr bool IS_3D_MODEL = false; // 3dモデルを用いて描画を行うかどうか
	GameObject()
	{
		// タグの配列リザーブ
		//m_Tag.reserve(4);

		if(m_NextObjectID >= UINT_MAX)
		{
			m_NextObjectID = 0; // オーバーフロー防止
		}
		m_ObjectID = m_NextObjectID++;
	}
	// ムーブコンストラクタ(改善版)
	GameObject(GameObject&& Other) noexcept
		: m_Transform(std::move(Other.m_Transform))
		, m_Velocity(std::move(Other.m_Velocity))
		, m_IsAliveData(std::exchange(Other.m_IsAliveData, false))
		, m_IsActive(std::exchange(Other.m_IsActive, false))
		, m_Destroy(std::exchange(Other.m_Destroy, false))
		, m_TextureID(std::exchange(Other.m_TextureID, -1))
		, m_Layer(std::exchange(Other.m_Layer, 0))
		, m_ObjectID(std::exchange(Other.m_ObjectID, 0))
		, m_Tag(std::move(Other.m_Tag))
		, m_ObjSpeedMlt(std::exchange(Other.m_ObjSpeedMlt, 1.0f))
		, m_VertexBuffer(std::exchange(Other.m_VertexBuffer, nullptr))
		, m_VertexShader(std::exchange(Other.m_VertexShader, nullptr))
		, m_PixelShader(std::exchange(Other.m_PixelShader, nullptr))
		, m_VertexLayout(std::exchange(Other.m_VertexLayout, nullptr))
	{

	}
	// ムーブ代入演算子(改善版)
	GameObject& operator=(GameObject&& Other) noexcept
	{
		if (this != &Other)
		{
			m_Transform = std::move(Other.m_Transform);
			m_Velocity = std::move(Other.m_Velocity);
			m_IsAliveData = Other.m_IsAliveData;
			m_IsActive = Other.m_IsActive;
			m_Destroy = Other.m_Destroy;
			m_TextureID = Other.m_TextureID;
			m_Layer = Other.m_Layer;
			m_ObjectID = Other.m_ObjectID;
			m_Tag = std::move(Other.m_Tag);
			m_ObjSpeedMlt = Other.m_ObjSpeedMlt;
			m_VertexBuffer = Other.m_VertexBuffer;
			m_VertexShader = Other.m_VertexShader;
			m_PixelShader = Other.m_PixelShader;
			m_VertexLayout = Other.m_VertexLayout;

			Other.m_VertexBuffer = nullptr;
			Other.m_VertexShader = nullptr;
			Other.m_PixelShader = nullptr;
			Other.m_VertexLayout = nullptr;
			Other.m_TextureID = -1;
		}
		return *this;
	}

	virtual ~GameObject() {}
	template <typename... Args>
	void Init(Args&&... args) 
	{
		
	};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void UpdateGPUData(InstanceBufferData& data); // GPUバッファ更新関数
	void StackDrawCall();
	virtual void Draw() {};

	void SetCanChangeVertex(bool is2D); // 頂点データが変更可能にできるプリセット
	void ChangeTexUV(int texWidthCount, int texHeightCount, int widthTarget, int heightTarget, bool is2D);
	Vector2 CalcTexUVOffset(int texWidthCount, int texHeightCount, int widthTarget, int heightTarget) const;

	inline void SetVertexBuffer(ID3D11Buffer* VertexBuffer) { m_VertexBuffer = VertexBuffer; }
	inline ID3D11Buffer* GetVertexBuffer() const { return m_VertexBuffer; }

	// get/set系関数(軽いものはinlineをつけ、get/setの適切な部分にconstをつけること!)
	inline void SetLayer(const int& layer) { m_Layer = layer; }
	inline const int& GetLayer() const { return m_Layer; }
	inline void SetIsAlive(const bool& manage) { m_IsAliveData = manage; }
	inline const bool& GetIsAlive() const { return m_IsAliveData; }
	inline void SetObjectID(const unsigned int& id) { m_ObjectID = id; }
	inline const unsigned int& GetObjectID() const { return m_ObjectID; }
	void AddPosition(const Vector3& Position, const bool& calcWorldSpeed = true);
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
	inline std::deque<std::string>& GetTagList() { return m_Tag; }
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
	// 単一検索
	virtual GameObject* FindObjectByTag(const std::string& tag)
	{
		for (auto& t : m_Tag)
		{
			if(t == tag)
			{
				return this; // タグが見つかったら自分自身を返す
			}
		}
		return nullptr;
	}
	// 複数検索
	virtual void FindObjectsByTag(const std::string& tag, std::list<GameObject*>& outList)
	{
		for (auto& t : m_Tag)
		{
			if(t == tag)
			{
				outList.push_back(this); // タグが見つかったら自分自身をリストに追加
				return;
			}
		}
	}


	inline void SetObjectSpeedMlt(const float& speedMlt) { m_ObjSpeedMlt = speedMlt; }
	inline const float GetObjectSpeedMlt() const { return m_ObjSpeedMlt; }
	inline const bool IsActive() const { return m_IsActive; }
	inline void SetActive(const bool& IsActive) { m_IsActive = IsActive; }
	inline void SetDestroy(const bool& Destroy) { m_Destroy = Destroy; }
	inline const bool IsDestroy() const { return m_Destroy; }
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