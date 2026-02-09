#pragma once

#include <vector>
#include <memory>
#include <algorithm>
// debugmsg用
#include <typeinfo>
#include <string>
#include "Windows.h"

#include "object/game_object.h"

class Panel; // 前方宣言


// ObjectManagerのインターフェースとなる基底クラス
class IObjectManager
{
public:
	virtual ~IObjectManager() = default;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void UpdateObjectByTag(const std::string& tag) = 0;
	virtual void UpdateObjectByTags(const std::list<std::string>& tags) = 0;
	// 破棄関数
	virtual void RemoveDestroyedObjects() = 0;
	virtual void RemoveAllObjects() = 0;
	// リザーブ関数
	virtual void Reserve(size_t capacity) = 0;
};

// ゲームオブジェクト用の機能を定義するインターフェースクラス
class IGameObjectManager : public IObjectManager
{
public:
	// インスタンシングレンダリングにて用いるgpu受け渡し用バッファの構造体
	struct InstanceBufferData
	{
		XMFLOAT4 Position; // xyz
		XMFLOAT4 Scale; // xyz
		XMFLOAT4 Color; // rgba
		XMFLOAT4 UVOffset; // xy: offset, zw: scale
	};

	struct RenderQueueData
	{
		int Layer; // layerNo
		float Depth; // カメラからの距離
		std::function<void()> DrawCall; // 実際の描画関数

		// 自作構造体なため、ソート用の比較演算子を定義
		bool operator<(const RenderQueueData& other) const
		{
			if (Layer != other.Layer)
			{
				return Layer < other.Layer; // レイヤーで比較
			}
			return Depth < other.Depth; // レイヤーが同じ場合は深度で比較
		}
	};

	virtual void UpdateGPUData() = 0;

	virtual GameObject* GetObjectByTag(const std::string& tag) = 0;
	virtual std::list<GameObject*> GetObjectsByTag(const std::string& tag) = 0;
	virtual void FlushPendingObjects() = 0;
	virtual void SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue) = 0;
	virtual void Draw() = 0;
	virtual void DrawObjectByTag(const std::string& tag) = 0;
	virtual void DrawObjectByTags(const std::list<std::string>& tags) = 0;
};

// システムオブジェクト用の機能を定義するインターフェースクラス
class ISystemObjectManager : public IObjectManager
{
public:
	virtual void UpdateFinal() = 0;
};

	

template <typename ObjectType>
class ObjectManager : public IGameObjectManager
{
public:
	ObjectManager()
	{
		m_Objects.reserve(ObjectType::MAX_OBJECTS);
		m_InstanceDataBuffer.reserve(ObjectType::MAX_OBJECTS);
	}

	// タグを用いた単体取得
	GameObject* GetObjectByTag(const std::string& tag) override
	{
		for (auto& obj : m_Objects)
		{
			GameObject* found = obj.FindObjectByTag(tag);
			if (found)
			{
				return found;
			}
		}
		return nullptr;
	}
	
	// タグを用いた複数取得
	std::list<GameObject*> GetObjectsByTag(const std::string& tag) override
	{
		std::list<GameObject*> result;
		for (auto& obj : m_Objects)
		{
			obj.FindObjectsByTag(tag, result);
		}
		return result;
	}

	// idを用いた単体取得
	ObjectType* GetObjectById(const unsigned int& id)
	{
		for (auto& obj : m_Objects)
		{
			if (obj.GetObjectID() == id)
			{
				return &obj;
			}
		}
		return nullptr;
	}

	void RemoveDestroyedObjects() override
	{
		for (auto& object : m_Objects)
		{
			if (object.IsDestroy())
			{
				object.Uninit();
			}
		}

		std::erase_if(m_Objects, [](const ObjectType& obj) 
		{
			return obj.IsDestroy(); // 削除する条件
		});
	}

	void RemoveAllObjects() override
	{
		// 終了処理実行
		for(auto& object : m_Objects)
		{
			object.Uninit();
		}
		// リストから消す
		m_Objects.clear();
	}


	void Reserve(size_t capacity)
	{
		m_Objects.reserve(capacity);
		m_InstanceDataBuffer.reserve(capacity);
	}

	template<typename... Args>
	// ここで返ったポインタを保存しないように!
	ObjectType* AddObject(int index, unsigned int objId, Args&&... args)
	{
		// 待機リストに追加
		auto& obj = m_PendingObjects.emplace_back();
		obj.SetObjectID(objId);
		obj.Init(std::forward<Args>(args)...);
		return &obj;
	}

	void FlushPendingObjects() override
	{
		if(m_PendingObjects.empty())
		{
			return; // 保留中のオブジェクトがなければ何もしない
		}
		// 保留中のオブジェクトを本体リストに移動
		for (auto& obj : m_PendingObjects)
		{
			// キャパシティチェック
			if (m_Objects.size() >= m_Objects.capacity())
			{
				// エラーメッセージを作成
				std::string errorMsg = "[Error] ObjectManager capacity exceeded! Target Type: ";
				errorMsg += typeid(ObjectType).name();
				errorMsg += "\nWrite more value on hedder!\n";

				// Visual Studioの出力ウィンドウに表示
				OutputDebugStringA(errorMsg.c_str());

				// 停止
				assert(!"Capacity exceeded! Check Output Window for the object type.");
			}
			m_Objects.emplace_back(std::move(obj));
		}
		m_PendingObjects.clear();
	}

	ObjectType* GetGameObject()
	{
		if (m_Objects.empty())
		{
			return nullptr;
		}
		return &m_Objects[0];
	}

	std::vector<ObjectType>& GetGameObjects()
	{
		return m_Objects;
	}

	// 

	void Init()
	{

	};
	void Uninit() override
	{
		// uninitする
		for(auto& obj : m_Objects)
		{
			obj.Uninit();
		}

		// リストから消す
		m_Objects.clear();
	};
	void Update() override
	{
		// 自身が所有しているオブジェクトを全て更新
		int index = 0;
		for(auto& obj : m_Objects)
		{
			obj.Update();
		}
	}

	// 全部のupdate終わった後に呼ぶことで他オブジェクトによって座標が動かされても大丈夫
	void UpdateGPUData() override
	{
		// AI提案まま
		if constexpr (ObjectType::ENABLE_INSTANCING)
		{
			m_InstanceDataList.clear();

			// 1. まずアクティブなオブジェクトへのポインタを集める
			std::vector<ObjectType*> activeObjects;
			for (auto& obj : m_Objects) 
			{
				if (obj.IsActive() && !obj.IsDestroy()) 
				{
					activeObjects.push_back(&obj);
				}
			}

			// 2. レイヤー順（昇順）にソートする
			// これにより、GPUバッファ内でレイヤーごとにデータが連続するようになる
			std::sort(activeObjects.begin(), activeObjects.end(),
				[](const ObjectType* a, const ObjectType* b) 
				{
					return a->GetLayer() < b->GetLayer();
				});

			// 3. ソート順通りにGPU用データを作成
			for (auto* obj : activeObjects) 
			{
				m_InstanceDataList.emplace_back(obj->GetInstanceData());
			}

			// 4. GPUバッファ転送 (Map/Unmap)
			// ... (省略: 前回のコードと同じ) ...
		}


		// 過去のやつ
		int index = 0;
		for(auto& obj : m_Objects)
		{
			obj.UpdateGPUData(m_InstanceBuffer, m_InstanceDataBuffer[index]);
		}
		// バッファ更新
		Renderer::GetDeviceContext()->UpdateSubresource(m_InstanceBuffer, 0, NULL, m_InstanceDataBuffer.data(), 0, 0);
	}

	void UpdateObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for(auto& obj : m_Objects)
		{
			if(obj.IsTagAvailable(tag))
			{
				obj.Update();
			}
		}
	}

	void UpdateObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for(auto& obj : m_Objects)
		{
			for(const auto& tag : tags)
			{
				if(obj.IsTagAvailable(tag))
				{
					obj.Update();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}

	void SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue) override
	{
		// インスタンシングレンダリングの場合は処理を追加(AIまるまるコピーなので検証してね)
		// === インスタンシングの場合 ===
		if constexpr (requires { ObjectType::ENABLE_INSTANCING; }&& ObjectType::ENABLE_INSTANCING)
		{
			if (m_InstanceDataList.empty()) return;

			// ソート済みの m_InstanceDataList を走査し、レイヤーの切れ目を見つけてリクエストを発行する
			// activeObjectsはUpdateGPUDataでソート済みなので、同じレイヤーは連続している前提

			int currentStartIndex = 0;
			int currentLayer = -9999; // ありえない値

			// レイヤー情報を復元するために、再度m_Objectsを走査するのは非効率なので、
			// UpdateGPUDataでソートした時の情報をキャッシュしておくか、
			// InstanceData構造体にLayerを含めるのが簡単です（描画には使いませんがcpu側で使う）。

			// ★簡略化のため、再走査ロジックのイメージで書きます
			// 実際は UpdateGPUData で「どの範囲がどのレイヤーか」のリストを作っておくのがベストです。

			struct BatchRange { int Layer; int Start; int Count; };
			std::vector<BatchRange> batches;

			// バッチ情報の構築 (UpdateGPUData内でやるとより高速)
			// ※ここでは概念説明のため都度計算します
			std::vector<ObjectType*> sortedObjs;
			for (auto& obj : m_Objects) if (obj.IsActive() && !obj.IsDestroy()) sortedObjs.push_back(&obj);
			std::sort(sortedObjs.begin(), sortedObjs.end(), [](auto* a, auto* b) { return a->GetLayer() < b->GetLayer(); });

			if (sortedObjs.empty()) return;

			int start = 0;
			int layer = sortedObjs[0]->GetLayer();
			for (int i = 0; i < sortedObjs.size(); ++i)
			{
				if (sortedObjs[i]->GetLayer() != layer)
				{
					// レイヤーが変わったので前のバッチを登録
					batches.push_back({ layer, start, i - start });
					layer = sortedObjs[i]->GetLayer();
					start = i;
				}
			}
			// 最後のバッチ
			batches.push_back({ layer, start, (int)sortedObjs.size() - start });


			// リクエストの発行
			for (const auto& batch : batches)
			{
				RenderRequest req;
				req.Layer = batch.Layer;
				req.Depth = 0.0f; // インスタンシング内での深度ソートはZバッファ任せ

				// 描画関数の登録（ラムダ式で値をキャプチャする） ->個別のオブジェクトごとに値を変えたいならここに各オブジェクトのDraw関数を呼ぶ?
				req.DrawCall = [this, batch]() {
					ObjectType::SetPipelineState();

					UINT strides[2] = { sizeof(VERTEX_3D), sizeof(typename ObjectType::InstanceData) };
					UINT offsets[2] = { 0, 0 };
					ID3D11Buffer* pBuffers[2] = { m_Objects[0].GetVertexBuffer(), m_InstanceBuffer };

					Renderer::GetDeviceContext()->IASetVertexBuffers(0, 2, pBuffers, strides, offsets);
					Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

					// ★ここが重要: DrawInstancedの引数で「開始位置(StartInstanceLocation)」を指定
					// 引数: VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation
					Renderer::GetDeviceContext()->DrawInstanced(4, batch.Count, 0, batch.Start);
					};

				renderQueue.push_back(req);
			}
		}
		// === インスタンシングではない場合 ===
		else
		{
			// 個別にリクエストを投げる
			for (auto& obj : m_Objects)
			{
				if (obj.IsActive() && !obj.IsDestroy())
				{
					RenderRequest req;
					req.Layer = obj.GetLayer(); // GameObjectにGetLayerが必要
					// カメラからの距離を計算（簡易）
					req.Depth = obj.GetPosition().z;

					req.DrawCall = [&obj]() {
						obj.Draw();
						};
					renderQueue.push_back(req);
				}
			}
		}
	}

	void Draw() override
	{
		// とりあえず所有しているオブジェクトを描画
		// 後にインスタンシングレンダリングに対応する形で

		// そうなるとレイヤーはここでvector<vector>にしないとダメやなぁ

		for(auto& obj : m_Objects)
		{
			obj.Draw();
		}
	};

	void DrawObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ描画
		for(auto& obj : m_Objects)
		{
			if(obj.IsTagAvailable(tag))
			{
				obj.Draw();
			}
		}
	}

	void DrawObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ描画
		for(auto& obj : m_Objects)
		{
			for(const auto& tag : tags)
			{
				if(obj.IsTagAvailable(tag))
				{
					obj.Draw();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}


private:
	std::vector<ObjectType> m_Objects;
	std::vector<ObjectType> m_PendingObjects; // 追加待ちオブジェクトのリスト
	// インスタンシングレンダリング用
	ID3D11Buffer* m_InstanceBuffer = nullptr;
	ID3D11ShaderResourceView* m_InstanceSRV = nullptr;
	std::vector<InstanceBufferData> m_InstanceDataBuffer;
};

template <typename ObjectType>
class SystemObjectManager : public ISystemObjectManager
{
public:
	SystemObjectManager()
	{
		m_Objects.reserve(ObjectType::MAX_OBJECTS);
	}

	void RemoveDestroyedObjects() override
	{
		// 何もしない
	}

	void RemoveAllObjects() override
	{
		// 終了処理実行
		for (auto& object : m_Objects)
		{
			object.Uninit();
		}
		// リストから消す
		m_Objects.clear();
	}

	void Reserve(size_t capacity)
	{
		m_Objects.reserve(capacity);
	}

	template<typename... Args>
	ObjectType* AddObject(Args&&... args)
	{
		// キャパシティチェック
		if (m_Objects.size() >= m_Objects.capacity())
		{
			// エラーメッセージを作成
			std::string errorMsg = "[Error] ObjectManager capacity exceeded! Target Type: ";
			errorMsg += typeid(ObjectType).name();
			errorMsg += "\nWrite more value on hedder!\n";

			// Visual Studioの出力ウィンドウに表示
			OutputDebugStringA(errorMsg.c_str());

			// 停止
			assert(!"Capacity exceeded! Check Output Window for the object type.");
		}

		// vectorのメモリ上でオブジェクトを構築(emplace_backはc++17から参照を返す)
		auto& obj = m_Objects.emplace_back();
		obj.Init(std::forward<Args>(args)...);

		return &obj;
	}

	ObjectType* GetSystemObject()
	{
		if (m_Objects.empty())
		{
			return nullptr;
		}
		return &m_Objects[0];
	}

	std::vector<ObjectType>& GetSystemObjects()
	{
		return m_Objects;
	}

	void Init()
	{

	}
	void Uninit() override
	{
		for (auto& object : m_Objects)
		{
			object.Uninit();
		}
		m_Objects.clear();
	}

	void Update() override
	{
		for (auto& object : m_Objects)
		{
			object.Update();
		}
	}

	void UpdateObjectByTag(const std::string& tag) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for (auto& obj : m_Objects)
		{
			if (obj.IsTagAvailable(tag))
			{
				obj.Update();
			}
		}
	}

	void UpdateObjectByTags(const std::list<std::string>& tags) override
	{
		// 指定タグを持つオブジェクトのみ更新
		for (auto& obj : m_Objects)
		{
			for (const auto& tag : tags)
			{
				if (obj.IsTagAvailable(tag))
				{
					obj.Update();
					break; // タグが見つかったら次のオブジェクトへ
				}
			}
		}
	}

	void UpdateFinal() override
	{
		for (auto& object : m_Objects)
		{
			object.UpdateFinal();
		}
	}

private:
	//int m_Id = 0;
	std::vector<ObjectType> m_Objects;
};