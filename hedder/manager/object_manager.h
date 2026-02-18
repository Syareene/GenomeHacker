#pragma once

#include <vector>
#include <memory>
#include <algorithm>
// debugmsg用
#include <typeinfo>
#include <string>
#include "Windows.h"
#include "object/game_object.h"
#include "object/i_container.h"
#include "object/check_override.h"
#include "object/3d_object.h"
#include "manager/texture_manager.h"
#include "lib/modelRenderer.h"

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


	virtual void UpdateGPUData() = 0;

	virtual GameObject* GetObjectByTag(const std::string& tag) = 0;
	virtual std::list<GameObject*> GetObjectsByTag(const std::string& tag) = 0;
	virtual void FlushPendingObjects() = 0;
	virtual void SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue, std::deque<std::string> tags) = 0;
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
		CheckOverride();
		m_Objects.reserve(ObjectType::MAX_OBJECTS);
		m_InstanceDataBuffer.reserve(ObjectType::MAX_OBJECTS);
		// shaderに渡したいプロパティをInstanceDataで渡す

		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(InstanceBufferData) * ObjectType::MAX_OBJECTS;
		bd.StructureByteStride = sizeof(InstanceBufferData);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 第二引数は初期データなので今回はnullでok
		Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_InstanceBuffer);

		// シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = ObjectType::MAX_OBJECTS;

		// 設定
		Renderer::GetDevice()->CreateShaderResourceView(m_InstanceBuffer, &srvd, &m_InstanceSRV);
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
		obj.SetLayer(index);
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
		if constexpr (ObjectType::ENABLE_INSTANCING)
		{
			// データをクリア(メモリ上でのサイズは変わらない)
			m_InstanceDataBuffer.clear();

			// まずアクティブなオブジェクトへのポインタを集める
			std::vector<ObjectType*> activeObjects;
			for (auto& obj : m_Objects) 
			{
				if (obj.IsActive() && !obj.IsDestroy()) 
				{
					activeObjects.push_back(&obj);
				}
			}

			// レイヤー順にソートする
			// これにより、GPUバッファ内でレイヤーごとにデータが連続するようになる
			std::sort(activeObjects.begin(), activeObjects.end(),
				[](const ObjectType* a, const ObjectType* b) 
				{
					return a->GetLayer() < b->GetLayer();
				});

			// ソート順通りにGPU用データを更新
			for (auto* obj : activeObjects) 
			{
				// 要素を構築しその場所を取得
				InstanceBufferData& data = m_InstanceDataBuffer.emplace_back();
				// 引数に渡して書き込みしてもらう
				obj->UpdateGPUData(data);
			}

			// データがないなら転送しない
			if (m_InstanceDataBuffer.empty()) return;

			// GPUバッファ転送(Map/Unmap)
			D3D11_MAPPED_SUBRESOURCE msr;
			Renderer::GetDeviceContext()->Map(m_InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			memcpy(msr.pData, m_InstanceDataBuffer.data(), sizeof(InstanceBufferData) * m_InstanceDataBuffer.size());
			Renderer::GetDeviceContext()->Unmap(m_InstanceBuffer, 0);
		}


		// 過去のやつ
		/*int index = 0;
		for(auto& obj : m_Objects)
		{
			obj.UpdateGPUData(m_InstanceBuffer, m_InstanceDataBuffer[index]);
		}*/
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

	void SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue, std::deque<std::string> tags) override
	{
		// インスタンシングレンダリングの場合は処理を追加
		// インスタンシングの場合
		if constexpr (requires { ObjectType::ENABLE_INSTANCING; }&& ObjectType::ENABLE_INSTANCING)
		{
			if (m_InstanceDataBuffer.empty()) return;

			// ソート済みの m_InstanceDataList を走査し、レイヤーの切れ目を見つけてリクエストを発行する
			// activeObjectsはUpdateGPUDataでソート済みなので、同じレイヤーは連続している前提

			int currentStartIndex = 0;
			int currentLayer = -9999; // ありえない値

			// レイヤーごとにバッチを構築するための構造体とリスト
			struct BatchRange { int Layer; int Start; int Count; };
			std::vector<BatchRange> batches;

			// バッチ情報の構築
			std::vector<ObjectType*> sortedObjs;
			for (auto& obj : m_Objects)
			{
				if (!obj.IsActive() || obj.IsDestroy())
				{
					// アクティブでないか削除済みならスキップ
					continue;
				}
				// タグを確認

				// tagsがemptyならそのまま追加
				if (tags.empty())
				{
					sortedObjs.push_back(&obj);
					continue;
				}
				// タグがあるならどれか1つでもあれば追加
				for (auto& tag : tags)
				{
					if (!obj.IsTagAvailable(tag))
					{
						// タグが見つからなければスキップ
						continue;
					}
					// タグが見つかったため追加しループを抜ける
					sortedObjs.push_back(&obj);
					break;
				}
			}
			// レイヤー順にソート
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
				RenderQueueData req;
				req.Layer = batch.Layer;
				req.Depth = 0.0f; // インスタンシング内での深度ソートはZバッファ任せ

				// 描画関数の登録（ラムダ式で値をキャプチャする） ->個別のオブジェクトごとに値を変えたいならここに各オブジェクトのDraw関数を呼ぶ?

				ObjectType* activeObj = sortedObjs[batch.Start]; // バッチの最初のオブジェクトを代表として取得


				if constexpr (requires { ObjectType::IS_3D_MODEL; }&& ObjectType::IS_3D_MODEL)
				{
					// 対象があるかどうか
					if (m_Objects.empty()) return;
					MODEL* modelData = m_Objects[0].GetModelRenderer()->GetModel();
					// 読み込めないならスキップ
					if (!modelData) return;

					// ラムダ式でキャプチャし登録
					req.DrawCall = [this, batch, modelData]()
						{
							ObjectType::SetPipelineState();

							// 頂点バッファ・インデックスバッファの設定
							UINT stride = sizeof(VERTEX_3D);
							UINT offset = 0;
							// インスタンシングデータ(m_InstanceBuffer)もスロット1にバインド
							//ID3D11Buffer* pBuffers[2] = { modelData->VertexBuffer, m_InstanceBuffer };
							//UINT strides[2] = { stride, sizeof(typename ObjectType::InstanceBufferData) }; // 構造体名注意
							//UINT offsets[2] = { 0, 0 };

							Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &modelData->VertexBuffer, &stride, &offset);
							Renderer::GetDeviceContext()->IASetIndexBuffer(modelData->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
							Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

							// ストラクチャードバッファ設定(下のvertexbufferからこっちに対してセットしたいね)
							Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_InstanceSRV);


							// ストラクチャードバッファ方式の場合は VSSetShaderResources などを使用してください
							// 以下は頂点バッファ方式(InputLayout方式)の例です

							// サブセットごとの描画ループ
							for (unsigned int i = 0; i < modelData->SubsetNum; i++)
							{
								// マテリアル設定 (定数バッファ)
								Renderer::SetMaterial(modelData->SubsetArray[i].Material.Material);

								// テクスチャ設定
								if (modelData->SubsetArray[i].Material.Texture)
								{
									Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &modelData->SubsetArray[i].Material.Texture);
								}

								// インデックス付きインスタンシング描画
								// 引数: IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation
								Renderer::GetDeviceContext()->DrawIndexedInstanced(
									modelData->SubsetArray[i].IndexNum,
									batch.Count,
									modelData->SubsetArray[i].StartIndex,
									0,
									batch.Start
								);
							}
						};
				}
				else
				{
					// ラムダでキャプチャ
					req.DrawCall = [this, batch, activeObj]() {
						ObjectType::SetPipelineState(); // **********実装予定の各GameObject派生クラスに作成するstatic関数 シェーダーやlayoutをセットする**********
						// これ対象が2dの場合はdepth enable/disable切り替え必要

						// バッファ設定
						UINT stride = sizeof(VERTEX_3D);
						UINT offset = 0;
						ID3D11Buffer* vb = m_Objects[0].GetVertexBuffer();
						Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

						Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

						if(activeObj)
						{
							// テクスチャセット
							ID3D11ShaderResourceView * texture = TextureManager::Get3DTexture(activeObj->GetTextureID());

							Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
						}
						MATERIAL material;
						material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f); // 基本テクスチャ貼るのでこのまんま
						material.TextureEnable = true;
						Renderer::SetMaterial(material); // マテリアルセット(テクスチャを有効化)

						// ストラクチャードバッファ設定(下のvertexbufferからこっちに対してセットしたいね)
						Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_InstanceSRV);

						// まとめて描画
						Renderer::GetDeviceContext()->DrawInstanced(4, batch.Count, 0, batch.Start);
						};
				}

				renderQueue.push_back(req);
			}

			// gameobjectの中に更にgameobjectを管理しているようなものの場合は中身に対して実行する
			// これちょっと書き方異なるか?
			if constexpr (ContainerObject<ObjectType>)
			{
				for(auto& obj : m_Objects)
				{
					if(obj.IsActive() && !obj.IsDestroy())
					{
						obj.SubmitDrawRequests(renderQueue, tags);
					}
				}
			}
		}
		// インスタンシングではない場合
		else
		{
			// 個別にリクエストを投げる
			for (auto& obj : m_Objects)
			{
				// アクティブでないまたは削除済みならスキップ
				if (!obj.IsActive() || obj.IsDestroy())
				{
					continue;
				}
				// 確認用フラグ
				bool tagMatch = false;

				// タグを確認
				// tagsがemptyならtrueに
				if (tags.empty())
				{
					tagMatch = true;
				}
				else
				{
					// タグがあるならどれか1つでもあればtrueに
					for(auto& tag : tags)
					{
						if (obj.IsTagAvailable(tag))
						{
							tagMatch = true;
							break; // タグが見つかったらループを抜ける
						}
					}
				}

				if(tagMatch)
				{
					RenderQueueData req;
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
#ifdef _DEBUG
		// ここで個別にレンダリングを呼ぶと、インスタンスレンダリングを弾とか敵に取り入れた意味がなくなってしまうため
		// 実行速度の為にコメントアウトします。描画したい時は有効化してね

		// object3Dのみコライダ描画対応
		//if constexpr(std::is_base_of<Object3D, ObjectType>::value)
		//{
		//	// デバッグ時にコライダを描画
		//	const int DEBUG_LAYER = 1000;
		//	for (auto& obj : m_Objects)
		//	{
		//		// アクティブでないか削除済みならスキップ
		//		if (!obj.IsActive() || obj.IsDestroy()) return;
		//		// コライダないならスキップ
		//		if (!obj.GetCollider()) return;

		//		// 描画関数をラムダで登録
		//		RenderQueueData req;
		//		req.Layer = DEBUG_LAYER; // 最前面に描画
		//		req.Depth = 0.0f;
		//		req.DrawCall = [&obj]()
		//			{
		//				obj.DrawCollider();
		//			};
		//		renderQueue.push_back(req);
		//	}
		//}
#endif
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

	// このエラーでないのそれ以前の問題とかそういうことかね
	void CheckOverride()
	{
		if constexpr (requires { ObjectType::ENABLE_INSTANCING; }&& ObjectType::ENABLE_INSTANCING)
		{
			// static関数はvirtual化できないためここでチェックする
			// GameObjectにSetPipelineState関数を定義していないとコンパイルエラーを出す
			static_assert(HasPipelineState<ObjectType>,
				"Error: This class enables instancing but forgot to define 'static void SetPipelineState()'.");
		}
	}
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